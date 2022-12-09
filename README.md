# Icon Overlays on Windows

Trying to get icon overlays to work on Windows.

- `vs/` using ATL.
- `py/` using Python.
- `rs/` DLL but using Rust.

# Docs

- https://learn.microsoft.com/en-us/windows/win32/shell/icons-and-icon-overlays-bumper
- https://scorpiosoftware.net/2021/12/11/icon-handler-with-atl/
- https://github.com/zodiacon/DllIconHandler
- http://timgolden.me.uk/python/win32_how_do_i/add-my-own-icon-overlays.html

# VS

Open `vs/ParsecShell.sln` with Visual Studio. Build the project.

```
> regsvr32 /n /i:user x64\Debug\ParsecShell.dll
```

Restart the explorer through the task manager and retry the previous command until it works.

