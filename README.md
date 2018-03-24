# Quark
Create Applications with browser technologies using the native engine in your OS.

## What is This?
After seeing this awesome project here on GitHub: https://github.com/pojala/electrino I decided to revitalize the project.
Currently, the only code written is a simple starter for the Window's side of things. It creates a window, initializes the CharkCore, and p
prints out a simple executed script that was created by us.

## Goals
I am hoping to make this a cross OS solution, similar to the APIs for NWjs: https://nwjs.io/ or Atom: https://atom.io/
I realize that this is a lofty goals, but there is always a need for competition.

## Design
The design of Quark is to use as little memory as possible. By utilizing the in memory browser instance that each OS has, we can
lower the footprint down to kilobytes of memory instead of megabytes.

This will mean dynamically linking into the browser instance and a corresponding Script engine to get at system resources.
As stated above, I currently will only be developing on windows, but will branch once I feel that there is a good layer and API
made for this project. This means a MVP needs to be created.

If there are any ideas of what this MVP may look like, I am always willing to take suggestions, but I am really looking for the bare
minimum that Atom and NW give.

## FAQ

1. Why create a new web container instead of using those that are already out there?
Well, as stated above, the current web containers do not have low memory consumption, so as the create of Electrino pointed out,
there needs to be a nice simple wrapper created for those that need low memory consumption.

2. Why is the Windows version being written first?
I have felt that dealing with the Windows API, especially in C++ has always been difficult for me, so I decided to tackle this challenge
first. I feel that I have a good enough grasp of the Unix style of programming, even if OSX deals with different pieces in a different way,
I feel it will still be easier to write for than Windows.

3. Why C++ and not Rust, Go, etc.?
Believe me, I struggled with this for a while and maybe I can eventually bind wrappers so that development of this system can be even faster. But,
at the current time, I will be developing in C++ and try to stick to the C++14 standard (I am learning as I go so I know it will not fully adhere to it).

4. When will this be done?
Who knows, this is not even in alpha state yet, but I do plan on keeping up with this project as I have a personal incentive to want this done.
Web containers are a wonderful technology and I do believe that it has helped move GUI development forward, but our current instances are very bloated.
I wish to see these containers move to a lower level.

5. Can I get involved?
Of course! I definitely want any and all contributors to this project! I would advise that if you want to start building the OSX or Linux verison of this that
we will need to discuss the overarching API that we will call since I do not have this planned out yet.

6. So what else?
Honestly, I really want this to become something and I am willing to put in the time, but do realize that I have a full time job, along with another project that
I am working on (an OS written Rust because why not). I will be splitting my time between the two, but I will make sure this stays updated. At least working on it once or twice a week.

# Current Footprint
Quark, with ChakraCore and a full IE11 browser is sitting at 7MB of RAM usage with the debugger attached. It is also sitting at 198KB for an exe and 5 MB for the ChakraCore.dll of disk usage. I will do some more testing around it without Chakra or with V8. There will most likely be different configurations that will allow people to create very simple applications. More info on footprint coming in the near future!
