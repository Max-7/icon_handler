use std::sync::atomic::{AtomicBool, Ordering};

use lazy_static::lazy_static;
use windows::{
    core::{GUID, HRESULT, PCWSTR},
    Win32::Foundation::{CLASS_E_CLASSNOTAVAILABLE, S_FALSE, S_OK},
};

#[cfg(not(windows))]
compile_error!("Platform is not Windows!");

mod icon_handler;

#[no_mangle]
#[allow(non_snake_case)]
pub extern "stdcall" fn DllCanUnloadNow() -> HRESULT {
    let outstanding_count = icon_handler::factory::OUTSTANDING_OBJECTS.load(Ordering::Relaxed);
    let locked_count = icon_handler::factory::DLL_LOCKED.load(Ordering::Relaxed);

    if outstanding_count > 0 || locked_count > 0 {
        S_FALSE
    } else {
        S_OK
    }
}

#[no_mangle]
#[allow(non_snake_case)]
pub unsafe extern "stdcall" fn DllGetClassObject(
    obj_guid: &GUID,
    factory_guid: &GUID,
    ppv: *mut *const std::ffi::c_void,
) -> windows::core::HRESULT {
    let clsid_parsec_icon_overlay_identifier =
        GUID::from(icon_handler::CLSID_PARSEC_ICON_OVERLAY_IDENTIFIER);

    if clsid_parsec_icon_overlay_identifier == *obj_guid {
        icon_handler::factory::query_interface(
            &icon_handler::factory::IICON_HANDLER_FACTORY,
            factory_guid,
            ppv,
        )
    } else {
        *ppv = std::ptr::null();
        CLASS_E_CLASSNOTAVAILABLE
    }
}

#[no_mangle]
#[allow(non_snake_case)]
pub extern "stdcall" fn DllCanUnloadNow() -> windows::core::HRESULT {
    S_FALSE
}

#[no_mangle]
#[allow(non_snake_case)]
pub extern "stdcall" fn DllInstall(binstall: bool, pszCmdLine: PCWSTR) -> HRESULT {
    if !pszCmdLine.is_null() {
        todo!("Handle per user registration")
    }

    if binstall {
        todo!("Handle server registration");
    } else {
        todo!("Handle server Unregistration");
    }

    S_OK
}
