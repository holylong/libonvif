@REM cmake -GNinja -Bbuild_ninja -DOpenCV_DIR=E:\Librarys\opencv\build\ -DFFMPEG_DIR=%cd%\3rdparty\ffmpeg-n4.4.3
cmake -GNinja -Bbuild_ninja -DOpenCV_DIR=D:\commonlib\opencv\opencv4100\build -DFFMPEG_DIR=%cd%\3rdparty\ffmpeg-n4.4.3
cmake --build build_ninja 
