# FileSystem Assignment
Repository for CSC 415 FileSystem assignment. This repository represents a contained FileSystem executable runnable through gcc compilation with entry on main.c. 

## Structure
./ -> root files like readme, gitignore, cmakelists (clion make file)

./include -> All header files
./src -> for all source .c files
./src/obj -> for the generated output files

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
This can be located at https://github.com/pambalos/csc317-webapp/pulls
