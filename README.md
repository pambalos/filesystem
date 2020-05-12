# FileSystem Assignment
Repository for CSC 415 FileSystem assignment. This repository represents a contained FileSystem executable runnable through gcc compilation with entry on main.c. 

## Structure
./ -> root files like readme, gitignore, cmakelists (clion make file)

./include -> All header files
./src -> for all source .c files
./src/obj -> for the generated output files

## Launch Process
### CLion:
If using CLion, the CMakeLists.txt has the necessary configuration for running it within the IDE. 

In run configuration, the working directory should be: ./src

Program arguments can be: test 10000 512

### Makefile & GCC:
Another way to compile this project is through GCC and the ./src/Makefile

From ./src, run 'make', and this will compile the 'fsdriver3.c' executable, which cane be run as follows from ./src:

./driver [volume_name] [volume_size] [BlockSize] 

i.e.: 

./driver test 10000 512

once run, this will launch the filesystem executable which will look for a volume with that name, and open it if it exists, or create a new file system if it does not. 

From the terminal, run 'help' to see available commands.

an example is:

ls -a

mkdir td

cd td

touch t1

ls 

cd ..

ls

## Commit Process

### Please do not commit directly to the master branch.

Open a new branch with:

#### git checkout -b branchName

Then when you are ready to commit your changes, commit with a good message:

#### git commit -m 'your commit message, give details'

Then, you can push.

#### git push

If this last command fails, it is because your local branch is not in the repo. It will suggest an alternate command, go ahead and run that command.

Then to merge your work into master, we will open a Pull Request from the Github UI, merging each piece in as we go.
This can be located at https://github.com/pambalos/filesystem
