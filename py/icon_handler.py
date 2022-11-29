from win32com.shell import shell, shellcon
import winerror

class IconOverlay:
    _reg_clsid_ = '{7FAE189D-C543-4199-AC82-F11580D6D49F}'
    _reg_progid_ = 'IconOverlayHandler'
    _reg_desc_ = 'Icon Overlay Handler'
    _reg_threading_ = "both"
    _public_methods_ = ['GetOverlayInfo', 'GetPriority', 'IsMemberOf']
    _com_interfaces_ = [shell.IID_IShellIconOverlayIdentifier]
    
    def GetOverlayInfo(self):
        # Change me
        return ('D:\projects\icon_handler\check.ico', 0, shellcon.ISIOI_ICONFILE)
    
    def GetPriority(self):
        return 50
    
    def IsMemberOf(self, fname, attributes):
        if fname.endswith(".py"):
            return winerror.S_OK
        return winerror.E_FAIL

if __name__=='__main__':
    import win32api
    import win32con
    from win32com.server.register import RegisterClasses, UnregisterClasses, UseCommandLine

    UnregisterClasses(IconOverlay)
    UseCommandLine(IconOverlay)

    keyname = 'Software\Microsoft\Windows\CurrentVersion\Explorer\ShellIconOverlayIdentifiers\MycOverlay'
    try:
        win32api.RegDeleteKey(win32con.HKEY_LOCAL_MACHINE, keyname)
    except:
        pass
    key = win32api.RegCreateKey(win32con.HKEY_LOCAL_MACHINE, keyname)
    win32api.RegSetValue(key, None, win32con.REG_SZ, IconOverlay._reg_clsid_)