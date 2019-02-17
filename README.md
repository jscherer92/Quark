# Quark
Create Applications with browser technologies using the native engine in your OS.

## What is This?
After seeing this awesome project here on GitHub: https://github.com/pojala/electrino I decided to revitalize the project.
Currently, the only code written is a simple starter for the Window's side of things. It creates a window, initializes the CharkCore, and prints out a simple executed script that was created by us.

## Goals
I am hoping to make this a cross OS solution, similar to the APIs for NWjs: https://nwjs.io/ or Electron: https://electronjs.org/
I realize that this is a lofty goals, but there is always a need for competition.

## Design
The design of Quark is to use as little memory as possible. By utilizing the in memory browser instance that each OS has, we can
lower the footprint down to kilobytes of memory instead of megabytes.

This will mean dynamically linking into the browser instance and a corresponding Script engine to get at system resources.
As stated above, I currently will only be developing on Windows, but will branch once I feel that there is a good layer and API
made for this project. This means a MVP needs to be created.

If there are any ideas of what this MVP may look like, I am always willing to take suggestions, but I am really looking for the bare
minimum that Atom and NW give.

## FAQ

#### Why create a new web container instead of using those that are already out there?
Well, as stated above, the current web containers do not have low memory consumption, so as the create of Electrino pointed out,
there needs to be a nice simple wrapper created for those that need low memory consumption.

#### Why is the Windows version the first PoC?
I felt that dealing with the Windows API was always a challenge for me. Dealing with the COM system and fully understanding how they composed their items is still a tough task. But, after creating the PoC for this project, I have decided to move it in a different direction.

#### When will this be done?
Who knows, this is not even in alpha state yet, but I do plan on keeping up with this project as I have a personal incentive to want this done.
Web containers are a wonderful technology and I do believe that it has helped move GUI development forward, but our current instances are very bloated.
I wish to see these containers move to a lower level.

#### Can I get involved?
Of course! I definitely want any and all contributors to this project! I would advise that if you want to start building the OSX or Linux verison of this that we will need to discuss the overarching API that we will call since I do not have this planned out yet.

#### How Can I get Updates? Is the Project Dead?
No it is not dead and I will do my best to make sure it doesn't die (unless something else comes along that completely destroys this project). I will try to keep my Twitter alive (I am not the best at social medai, I am pretty sure some of my friends think I have died since I haven't posted on my social media for over a couple years). My bio has my twitter link, but here it is just in case https://twitter.com/JustinScherer7

#### So what else?
Honestly, I really want this to become something and I am willing to put in the time, but do realize that I have a full time job, along with another project that I am working on (an OS written Rust because why not). I will be splitting my time between the two, but I will make sure this stays updated. At least working on it once or twice a week.

# Current Footprint
Quark, with ChakraCore and a full IE11 browser is sitting at 9MB of RAM usage with the debugger attached and no object cleanup. It is also sitting at 198KB for an exe and 5 MB for the ChakraCore.dll of disk usage. I will do some more testing around it without Chakra or with V8. There will most likely be different configurations that will allow people to create very simple applications. More info on footprint coming in the near future!

# Update for the Future
So, after sometime of building the PoC in Windows and just generally getting my bearing on the C++11 and C++14 standard, I have decided to go in a different direction. This direction may alienate people from the project, but I do blieve that it will benefit the project in the long run.

After working with a couple modern system languages, Rust and Go (yes, I do consider Go a systems language), I had first decided to use C++. It was something that I knew quite well, but I also knew many of the shortcomings of it. After studying what I had built, the Chromium browser, the V8 engine, Charka, etc. it made me realize how much I did not enjoy writing in C++. While the challenges I faced were fun, tackling them in something that I did not enjoy writing in lead me to not want to work on the project.

So what does this mean? I have decided to use a newer language called Crystal: https://crystal-lang.org. I have had quite an enjoyable time with it and I do believe the syntax is way more inviting to write in than C++. On top of this, I do believe that it is going to a great language for systems programming and will further the development of this project quite well.

One area that this is going to sour the discussion, for now, is in the Window's development. At this point, I am going to halt the development for Windows and move over to MacOSX. Crystal still does not have all the pieces in place to make it work well in Windows and so I believe running in the Unix system will provide a better development experience. I will be following their development of the Window's branch and will start work once it is up and running.

The current PoC code will live on, and I will make sure that it stays there, but I will be making this change.

# Update Febuary 2019
It has still been my goal to figure out the best solution to this problem that currently plagues the hybrid desktop application ecosystem. While some of my interests have moved onto other fields, I have still been studying up on the best way to build this application.

As stated before, the PoC will live on if anyone wants to continue building out the necessary versions, but I do not believe this ground up approach is going to be the best use of time for anyone. This does showcase how to interface with the built-in explorer for Windows and how to execute C++/JavaScript code in a mixed environment. Building the necessary components has seemed quite a heavy workload.

On the use of Crystal for developing this application, I have their Windows development to be slow. I then moved onto Nim to see if this would a usable system. Currently, I still feel they are to early to develop on even though I enjoyed working with their language. My next approach is going to be using something more widely used. I think utilizing Qt may be the best move forward and I will look into using this toolkit to keep moving forward.
