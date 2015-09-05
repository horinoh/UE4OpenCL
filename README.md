# UE4OpenCL

* https://developer.nvidia.com/cuda-downloads を DL してインストール (download and install)

* コピーする (copy)
~~~
copy C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v7.0\include\*.h UE4OpenCL\ThirdParty\CUDA\v7.0\include\
copy C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v7.0\lib\x64\OpenCL.lib UE4OpenCL\ThirdParty\CUDA\v7.0\lib\x64\
~~~

* .uproject を右クリック(right click .uproject) → Generate Visual Studio project files → .sln を開いてビルド (open .sln and build)