THIS_DIR=$(dirname "$0")
echo THIS_DIR=$THIS_DIR
mkdir -p external
cd $THIS_DIR/external

git clone -b docking https://github.com/ocornut/imgui.git
cd imgui
git checkout 58eb40db76783f5da09e592ca3eb421f4f2197e3 # v1.88
cd ..

git clone -b SDL2 https://github.com/libsdl-org/SDL.git

# Clone imgui-node-editor
#git clone -b develop https://github.com/thedmd/imgui-node-editor.git        # official version
git clone -b fix_win_clipping https://github.com/pthom/imgui-node-editor.git # fork with fixes + workaround

