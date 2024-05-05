# VexLog

> NOTE: This repository is not affiliated nor endorsed by the Purdue Sigbots team or VEX Robotics. 

There's a small problem with PROS: There's not an easy logging library for it. It's great and all, but having a library that can be used to log timestamps and log levels is incredibly useful. So, I made this. 



# Using this Library:

You can declare `ROBOTLOG::LOGGER logger;` at the top of any file, and it *should* work fine. 
> [!CAUTION]
> DO NOT DECLARE THE NAME OF YOUR LOGGER AS info, warning, debug or error. I use #defines in the library, which enables the ability to log the current file, but in consequence can cause issues with variables named this way.

