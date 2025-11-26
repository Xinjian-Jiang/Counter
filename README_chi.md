# 计数器
主打一个无脑，一行一行复制粘贴到终端最后包能跑的
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
### 下载安装git等其他工具
如果一个机器连git都没有，那用户肯定有sudo，所以直接装就可以
```bash
sudo apt update
sudo apt install -y git build-essential cmake pkg-config
```
### 克隆这个项目
```bash
git clone --recursive ...
cd ~/Counter
```

## 编译
```bash
cd external/gbbs
bazel build //...
cd ../..
```
