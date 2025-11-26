# 计数器
（暂时性的中文）
一行一行复制粘贴到终端
## 1.准备构建工具
### 1.1.创建local文件夹
```bash
mkdir -p ~/local ~/download
mkdir -p ~/local/bin ~/local/lib ~/local/include ~/local/share
echo 'export PATH="$HOME/local/bin:$PATH"' >> ~/.bashrc
echo 'export LD_LIBRARY_PATH="$HOME/local/lib:$LD_LIBRARY_PATH"' >> ~/.bashrc
echo 'export CPATH="$HOME/local/include:$CPATH"' >> ~/.bashrc
echo 'export LIBRARY_PATH="$HOME/local/lib:$LIBRARY_PATH"' >> ~/.bashrc
source ~/.bashrc
```
### 1.2.下载安装bazel
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
## 2.克隆这个项目
```bash
git clone --recursive https://github.com/201220022/Counter.git && cd Counter
```
## 3.编译运行
cd 到MIS中任意一个文件夹
```bash
./example.sh
```
## 4.批量运行
cd到外面Counter/
创建python虚拟环境，并pip install所需的包，只要一次
```bash
python3 -m venv utils/python
source utils/python/bin/activate
pip install -r requirements.txt
deactivate
```
以后每次使用, cd到Counter/MIS/
```bash
./run.sh
```