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

# Update Febuary 2019
It has still been my goal to figure out the best solution to this problem that currently plagues the hybrid desktop application ecosystem. While some of my interests have moved onto other fields, I have still been studying up on the best way to build this application.

As stated before, the PoC will live on if anyone wants to continue building out the necessary versions, but I do not believe this ground up approach is going to be the best use of time for anyone. This does showcase how to interface with the built-in explorer for Windows and how to execute C++/JavaScript code in a mixed environment. Building the necessary components has seemed quite a heavy workload.

On the use of Crystal for developing this application, I have their Windows development to be slow. I then moved onto Nim to see if this would a usable system. Currently, I still feel they are to early to develop on even though I enjoyed working with their language. My next approach is going to be using something more widely used. I think utilizing Qt may be the best move forward and I will look into using this toolkit to keep moving forward.

# Update March 2019
Funny that I keep coming back to this project even after I think that I am done with it. I will be doing some code cleanup here and there and will keep moving forward with it. After spending enough time in other libraries including Qt and even looking at other languages such as Nim and Crystal, I keep coming back to plain old C++. I do plan to add in a helper library called Poco. It is under the Boost license which should make it better than utilizing Qt (under LGPLv2 if you do not want to use it for commercial use). 

There is only one piece of Poco that I am still debugging and that is the https server portion. I have been having some issues trying to get it to read the certificate that I hand it, but otherwise it is a great cross platform library that has really boosted some things that I was worried about doing for this project.

So this leaves me to move forward with this. I will continue at a pace that I think is comfortable for me. Once the first interface between Windows and the shared piece is done, I think it will move much faster than it has. I am a little bit wary of OSX, mainly because I have done almost zero work there. But, once this is at a MVP level, I think I will move over to that portion.

# Update July 2019
It has been quite some time since I have touched the project. To be honest, I have been working on something else that will be coming out next year and it has eaten quite a bit of my time sadly. I always keep coming back to this project since my day job involves me working in web containers and it is always sad when I have to do memory profiling and the base container is 50MB.

So, I have been revamping my personal schedule to help balance quite a few things in my life and one of them is spending time outside of this other project that I sadly can not talk about at this time. The next thing I will be looking at on the side is Vulkan. Some of you may be wondering, how does this relate to this project in any way? Well, there are a couple cool libraries that I think are going to help out in the development of this application.

1) GLFW, window creation without all of the boilerplate code that I have in the current project.
2) May be able to utilize Vulkan as a target instead of the web container (just a thought)
3) It is going to help my C++ writing capabilities. I am still shaky on some concepts with C++ and I want to make sure that I am delivering something that is going to be both usable but also easy to contribute to.

So, while this project will be slow going, I will also open up the other repository once I have learned more about Vulkan and have started to play with it more. That project should be up in the next month or two. Again, this is still not abandoned, it is more on hold while I am still trying to figure out how to both balance and build this project.

Thanks!

# Update January 2020
Well, I know I have been getting comments on where the status is for this project and I know that I have stated that I was going to try to work on it. But, I was also overly optimistic with other projects and this one. My current main project is almost finished, but that also means that I will need to start working on some other pieces (mainly learning some new tech since my current main project has eaten up most of my free time for the past year).

What is the status of all of this? Well, to be frank, there is not much progress. Microsoft has decided to go with the Chromium engine for their browser and that would hopefully mean that they will expose it as an API for the COM system. If not, well, we are going to have the same bloated interfaces that you get with NWJS or Electron sadly. 

There are other ways around this and they may end up working better, but they are monumental lifts all in their own. The main ones I can think of off of the top of my head are:

1) Implement a subset of the rendering system for HTML/CSS to be super lightweight. I know there are some projects our there such as:https://github.com/litehtml/litehtml that could achieve this. With this out of the way, it would putting a JavaScript engine with this. We could use V8, or we could use something lighter such as: https://github.com/jerryscript-project/jerryscript
2) Do something like above, but utilize Vulkan/OpenGL to do the rendering

I really don't know at the moment and these are all just ideas I have had while I have had downtime. Will I move forward with this project? I will probably wrap it up in a clean way so that writing C++ modules is a bit easier. But, after that, I think I will move onto my other projects for a bit and then potentially come back to this idea and implement, most likely, the first item.

We will see in the coming year what I eventually decide to do, but I thought it would be best to say something now since I have not done any work for the past year.

Thanks!
