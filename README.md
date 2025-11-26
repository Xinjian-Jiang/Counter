# 计数器
（暂时性的中文）
一行一行复制粘贴到终端
## 准备构建工具
### 创建local文件夹
```bash
mkdir -p ~/local ~/download
mkdir -p ~/local/bin ~/local/lib ~/local/include ~/local/share
echo 'export PATH="$HOME/local/bin:$PATH"' >> ~/.bashrc
echo 'export LD_LIBRARY_PATH="$HOME/local/lib:$LD_LIBRARY_PATH"' >> ~/.bashrc
echo 'export CPATH="$HOME/local/include:$CPATH"' >> ~/.bashrc
echo 'export LIBRARY_PATH="$HOME/local/lib:$LIBRARY_PATH"' >> ~/.bashrc
source ~/.bashrc
```
### 下载安装bazel
```bash
cd ~/download
wget https://github.com/bazelbuild/bazel/releases/download/7.7.1/bazel-7.7.1-installer-linux-x86_64.sh
chmod +x bazel-7.7.1-installer-linux-x86_64.sh
./bazel-7.7.1-installer-linux-x86_64.sh --prefix=$HOME/local
bazel --version
```
若显示
```bash
bazel 7.7.1
```
则安装成功
### 克隆这个项目
```bash
git clone --recursive https://github.com/201220022/Counter.git && cd Counter
```
## 编译运行
```bash
cd 到MIS任意一个文件夹./example.sh
```
