# VexLog

> NOTE: This repository is not affiliated nor endorsed by the Purdue Sigbots team or VEX Robotics. 

There's a small problem with PROS: There's not an easy logging library for it. It's great and all, but having a library that can be used to log timestamps and log levels is incredibly useful. So, I made this monstrosity. 

# How to Use this monstrosity
At the top of your C++ File, add the following code to the top of your main class below your includes:

```cpp
RobotLOG logger();
```

You can have it log to a file too:

```cpp
RobotLOG logger("out.txt"); // can be any extension or name, but txt is convenient
```

To log (since that's kind of the whole purpose of this) you can use 
```cpp
log(message);

//or

log(message);
```

