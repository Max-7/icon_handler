use widestring::{utf16str, Utf16Str};
use windows::{
    core::{PCWSTR, PWSTR},
    Win32::UI::Shell::{
        IShellIconOverlayIdentifier, IShellIconOverlayIdentifier_Impl, ISIOI_ICONFILE,
    },
};

// Win32 api needs wide strings (utf-16)
const ICON_PATH: &Utf16Str = utf16str!(r#"C:\Users\Corentin\Documents\icon-handler-rs\check.ico"#);
pub(crate) const CLSID_PARSEC_ICON_OVERLAY_IDENTIFIER: &str =
    "33338524-fc67-4bb9-9cd0-c46bca581343";

#[windows::core::implement(IShellIconOverlayIdentifier)]
pub(crate) struct ParsecIconHandler;

impl IShellIconOverlayIdentifier_Impl for ParsecIconHandler {
    fn IsMemberOf(&self, _pwszpath: &PCWSTR, _dwattrib: u32) -> windows::core::Result<()> {
        use windows::core;
        core::Result::Ok(())
    }

    fn GetOverlayInfo(
        &self,
        pwsziconfile: PWSTR,
        cchmax: i32,
        pindex: *mut i32,
        pdwflags: *mut u32,
    ) -> windows::core::Result<()> {
        // Assert: buffer is big enough
        assert!(cchmax > (ICON_PATH.len() + 1).try_into().unwrap());

        unsafe {
            pwsziconfile
                .as_ptr()
                .copy_from(ICON_PATH.as_ptr(), ICON_PATH.len() + 1);
            *pindex = 0i32;
            *pdwflags = ISIOI_ICONFILE;
        }

        windows::core::Result::Ok(())
    }

    fn GetPriority(&self) -> windows::core::Result<i32> {
        Ok(50)
    }
}

pub(crate) mod factory {
    use std::sync::atomic::{AtomicIsize, Ordering};

    use lazy_static::lazy_static;
    use windows::{
        core::{IUnknown, GUID, HRESULT},
        Win32::{
            Foundation::{CLASS_E_NOAGGREGATION, E_NOINTERFACE, E_OUTOFMEMORY},
            System::Memory::{GlobalAlloc, GMEM_FIXED},
            UI::Shell::{IShellIconOverlayIdentifier, IShellIconOverlayIdentifier_Vtbl},
        },
    };

    use super::ParsecIconHandler_Impl;

    pub const IICON_HANDLER_FACTORY: IIconHandlerFactory = IIconHandlerFactory {
        vtable: IICON_HANDLER_FACTORY_VTBL,
    };

    const IID_IUNKNOWN: GUID = GUID::from_u128(0u128);
    const IID_ICLASSFACTORY: GUID = GUID::from_u128(1u128);

    const IICON_HANDLER_FACTORY_VTBL: IIconHandlerFactoryVtbl = IIconHandlerFactoryVtbl {
        query_interface,
        add_ref,
        release,
        create_instance,
        lock_server,
    };

    lazy_static! {
        /// Atomic reference counter to track when the DLL can be freed in memory
        pub static ref OUTSTANDING_OBJECTS: AtomicIsize = AtomicIsize::new(0);

        /// Is the DLL locked in memory? Atomic counter to track memory lock
        /// If the DLL is locked by any application it can't be freed by the OS.
        pub static ref DLL_LOCKED: AtomicIsize = AtomicIsize::new(0);
    }

    #[repr(C)]
    pub struct IIconHandlerFactoryVtbl {
        query_interface: unsafe extern "system" fn(
            this: *const IIconHandlerFactory,
            factory_guiid: &GUID,
            ppv: *mut *const std::ffi::c_void,
        ) -> HRESULT,
        add_ref: unsafe extern "system" fn(this: *const std::ffi::c_void) -> u32,
        release: unsafe extern "system" fn(this: *const std::ffi::c_void) -> u32,
        create_instance: unsafe extern "system" fn(
            this: *mut IIconHandlerFactory,
            punk_outer: *mut IUnknown,
            vtable_guiid: &GUID,
            ppv: *mut *const std::ffi::c_void,
        ) -> HRESULT,
        lock_server:
            unsafe extern "system" fn(this: *mut IIconHandlerFactory, flock: bool) -> HRESULT,
    }

    #[repr(C)]
    pub struct IIconHandlerFactory {
        vtable: IIconHandlerFactoryVtbl,
    }

    /// This struct is just a helper to initialize a newly allocated struct for
    /// the IIconHandlerFactory CreateInstance function.
    /// This works because COM specify that the two first members of a C struct is
    /// a pointer to the vtable and an integer which represents a ref counter.
    /// We need this struct because the implements macro does not expose fields such as `vtable` or `count` publicly.
    #[repr(C)]
    struct DummyHandler {
        pub vtable: *const std::ffi::c_void,
        pub count: u32, // count is a DWORD
    }

    #[no_mangle]
    extern "system" fn add_ref(_this: *const std::ffi::c_void) -> u32 {
        1
    }

    #[no_mangle]
    extern "system" fn release(_this: *const std::ffi::c_void) -> u32 {
        1
    }

    #[no_mangle]
    pub unsafe extern "system" fn query_interface(
        this: *const IIconHandlerFactory,
        factory_guid: &GUID,
        ppv: *mut *const std::ffi::c_void,
    ) -> HRESULT {
        if *factory_guid != IID_IUNKNOWN && *factory_guid != IID_ICLASSFACTORY {
            *ppv = std::ptr::null();
            E_NOINTERFACE
        } else {
            *ppv = this as *const std::ffi::c_void;
            ((*this).vtable.add_ref)(this as *const std::ffi::c_void);
            HRESULT(0) // No error (S_OK)
        }
    }

    #[no_mangle]
    extern "system" fn lock_server(_this: *mut IIconHandlerFactory, flock: bool) -> HRESULT {
        if flock {
            DLL_LOCKED.fetch_add(1, Ordering::Relaxed);
        } else {
            DLL_LOCKED.fetch_min(1, Ordering::Relaxed);
        }

        HRESULT(0) // No error
    }

    #[no_mangle]
    unsafe extern "system" fn create_instance(
        _this: *mut IIconHandlerFactory,
        punk_outer: *mut IUnknown,
        vtable_guuid: &GUID,
        ppv: *mut *const std::ffi::c_void,
    ) -> HRESULT {
        *ppv = std::ptr::null();

        if !punk_outer.is_null() {
            CLASS_E_NOAGGREGATION
        } else {
            // This is a hack to get generated vtables by the `implement` proc macro
            let vtable = ParsecIconHandler_Impl::VTABLES.0;
            let allocated_memory = GlobalAlloc(
                GMEM_FIXED,
                std::mem::size_of::<IShellIconOverlayIdentifier>(),
            ) as *mut DummyHandler;

            (*allocated_memory).vtable =
                ((&vtable) as *const IShellIconOverlayIdentifier_Vtbl) as *const std::ffi::c_void;
            (*allocated_memory).count = 1;

            let qi_result = (vtable.base__.QueryInterface)(
                allocated_memory as *mut std::ffi::c_void,
                vtable_guuid,
                ppv,
            );
            if qi_result != HRESULT(0) {
                return qi_result;
            }
            (vtable.base__.Release)(allocated_memory as *mut std::ffi::c_void);

            if allocated_memory.is_null() {
                return E_OUTOFMEMORY;
            }

            // Increment the outstanding object count
            OUTSTANDING_OBJECTS.fetch_add(1, Ordering::Relaxed);

            HRESULT(0) // OK
        }
    }
}
