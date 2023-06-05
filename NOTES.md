
# Notes

These are my notes reflecting my thought process as I was working through this task.

## Understanding the task

> It is needed to create a cross-platform TCP server which calculates hash sums.

- My first thought is whether or not I can use external libraries
- If yes, then I can probably rely on my previous experience with Boost.Asio in rpclib
- If not, then I will need to do a bit of reading on the async socket APIs for Windows (and
hope I remember enough of the POSIX APIs to get by on Linux)

> A standard client application (e.g. telnet or netcat) connects to the server

- OK, this means I don't need to implement a client, neat
- I can probably also write integration tests easily

> via TCP and passes strings ending with newlines. The server must calculate hash sums of the
> strings (the type of the sum is at your discretion) and send them back in the hex format,
> finishing each with a newline. The strings can have arbitrary lengths.

- Since I'm adding Boost anyway as a dependency, I can use Boost to do the hashing as well as the
conversion into a hex string.
- It might be a good idea to implement a timeout in the server to prevent arbitrary-long inputs
inducing a denial-of-service attack

> Any open-source libraries can be used if desired.

- phew
- I'll likely use vcpkg instead of conan for simplicity
- At first I will just rely on my locally-installed dependencies

> The server must be built using CMake. At least Ubuntu 20 with GCC 11 and Windows 10 with Visual
> Studio 2022 must be supported. Both platforms must use Ninja as a build tool.

- nothing extraordinary here, I would have used CMake & ninja anyway
- if I have time, I might add a Github Actions build to verify portability across platforms

> The functional part of the server must be implemented as a dynamic library which must not export
> symbols considered private.

- Up until now I thought I was going to throw all this in a single cpp file, but it looks like one
of the objectives of the task is to demonstrate my ability and preferences around structuring a
more complex project.
- Regardless, the library will probably only export a single function and everything else will be
hidden
- To honor the compiler agnostic requirement, I'm going to use CMake's GenerateExportHeader module

> Units must have unit tests where reasonable, to be run via CTest.

- `enable_testing` then
- I'll probably use GoogleTest (even though I'm depending on Boost, but I'm more familiar with
gtest)

> All files must be accurately formatted using a style of your choice.

- My choice is a minimally configured clang-format

> A readme file must explain how to build and run the application in terms of prerequisites (if
> any). The source code must be put to any public repo.

- I'll probably use Github for this


## Setting up the project

My first steps were creating the repo, adding
  - .gitignore
  - README.md
  - .clang-format

After that, I started creating the CMake build. First, I created the `include`, `src`, `lib` and
`test` directories. Then I added the top-level CMakeLists.txt and started adding subdirectories.

I created the `serverlib` target `lib` directory and the `hss` (stands for "HaSh Server")
executable target in the `src` directory. In both cases I added an additional subdirectory and
used `target_sources` to add the source files. Admittedly, this is a bit of overkill for a project
this small, but this is to showcase my preferred way of structuring a project.

Then I added a little bit of boilerplate to both targets and wired up the export header.

This more or less covers my initial commit.


## A little more setup

I played around a bit with the structure (because I really wanted to use PUBLIC_HEADER to install
the header). Then I added a proper install target as well.

## Adding dependencies

I changed my mind about wiring up the dependencies later. Not that it matters much, but the
server implementation is going to be fairly trivial anyway, so why not get it out of the way.

I added the beginning of the build instructions, just so I don't forget later on. As the build is
running, I'm reminded why I dislike vcpkg haha. It's quite slow. But ultimately, this is probably
a good compromise for this project. I get a fairly easy setup for dependencies that will work
on both target platforms.

Since I was adding GoogleTest as well, I decided to set up the test project with a single
placeholder test and hooked it up to ctest.
