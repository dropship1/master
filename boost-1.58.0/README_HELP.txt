Make sure your directory structure looks like this.
bright/boost-1.58.0/boost

run boostrap.bat in root directory to build "bjam" tool.
http://stackoverflow.com/questions/8330302/trying-to-build-boost-i-cannot-get-bjam

Then do:
http://www.boost.org/doc/libs/1_58_0/doc/html/boost_asio/using.html
You may build the subset of Boost libraries required to use Boost.Asio and its examples by running the following command from the root of the Boost download package:
bjam toolset=msvc --with-system --with-thread --with-date_time --with-regex --with-serialization stage variant=release,debug


You really just need to extract the libs from stage/lib dir and place them in bright/lib.
The only files that are needed as reference are in boost  



You can download boost from here:
http://sourceforge.net/projects/boost/files/boost/1.58.0/
or from our google drive: boost_1_58_0.zip


This is what you should have under the repository direcotry boost-1.58.0 after extraction:
boost-1.58.0/README_HELP.txt
boost-1.58.0/bin.v2
boost-1.58.0/stage
boost-1.58.0/project-config.jam
boost-1.58.0/bootstrap.log
boost-1.58.0/b2.exe
boost-1.58.0/bjam.exe
boost-1.58.0/tools
boost-1.58.0/status
boost-1.58.0/more
boost-1.58.0/libs
boost-1.58.0/doc
boost-1.58.0/boost
boost-1.58.0/boost.css
boost-1.58.0/boost.png
boost-1.58.0/boost-build.jam
boost-1.58.0/boostcpp.jam
boost-1.58.0/bootstrap.bat
boost-1.58.0/bootstrap.sh
boost-1.58.0/index.htm
boost-1.58.0/index.html
boost-1.58.0/INSTALL
boost-1.58.0/Jamroot
boost-1.58.0/LICENSE_1_0.txt
boost-1.58.0/rst.css
