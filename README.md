# VexLog

> NOTE: This repository is not affiliated nor endorsed by the Purdue Sigbots team or VEX Robotics. 

There's a small problem with PROS: There's not an easy logging library for it. It's great and all, but having a library that can be used to log timestamps and log levels is incredibly useful. So, I made this. 



# Using this Library:

You can declare `ROBOTLOG::LOGGER logger;` at the top of any file, and it *should* work fine. 
> [!CAUTION]
> DO NOT DECLARE THE NAME OF YOUR LOGGER AS info, warning, debug or error. I use #defines in the library, which enables the ability to log the current file, but in consequence can cause issues with variables named this way.

I'll be using <logobjname> in place of an actual name for the rest of this README, you need to give it an actual name for the logger object. using `ROBOTLOG::LOGGER logger;` will work fine in this case.


There are 6 different default levels. 

| Level |                                   Description                                    |
| ----- | -------------------------------------------------------------------------------- |
| DATA  | Data only logging, does not add *any* formatting                                 |
| DEBUG | for debugging only                                                               |
| INFO  | Informational logging                                                            |
| WARN  | Warnings, could be used if a robot doesn't hit a certain point by a certain time |
| ERROR | Errors, doesn't stop a user program.                                             |
| CUST  | Custom level, defined by ROBOTLOG::Level(your level here)                        |

The methods meant to be used by a user were written with #define(s) to allow for automatic file and line number inclusion. If you wish to lie to the logger about the line and file, you can bypass the methods listed at the bottom and instead directly call addlog(Level, Message, File, Line). This is what the #define(s) are replaced with. 

There are six (define) methods (not addlog) that are meant to be used in a user program.

.log(Level, Message). (Level needs to be either Level::(Level from the table above), or Level::(Custom Level). Message *should* allow for any data type to be passed in, though to add strings to other data types you may need to use std::to_string(non string data) + "your string".)

.data(Message)
.debug(Message)
.info(Message)
.warn(Message)
.error(Message)

The above are (pretty) self explanatory, with message being the message you want to log. 




## Nerd Statistics
I timed it, and (if I set it up right) it would seem that when you call a log function from above, it usually takes about 10 microseconds to run. I'd consider that pretty good. This is because I make a LogMessage struct, then add this struct to the queue. A seperate task reads this message and prints it to the console. I don't know how using a seperate task affects performance though. 

Also fun minor detail, using this method should prevent text getting jumbled together when outputting since it prints each line one by one in a seperate task. I don't know if the queues built into C++ are threadsafe or not, but if you know of one that blocks the user code as minimally as possible, feel free to open an issue suggestion.