THIS_DIR=$(dirname "$0")
echo THIS_DIR=$THIS_DIR
mkdir -p external
cd $THIS_DIR/external

git clone -b docking https://github.com/ocornut/imgui.git
git clone -b master https://github.com/pthom/hello_imgui.git

# Clone imgui-node-editor
#git clone -b develop https://github.com/thedmd/imgui-node-editor.git        # official version
git clone -b fix_win_clipping https://github.com/pthom/imgui-node-editor.git # fork with fixes + workaround
