meson x --buildtype release --strip -Db_lto=true
ninja -Cx
# meson x --buildtype release --strip -Db_lto=true \
#     -Dprebuilt_server=/path/to/scrcpy-server
# ninja -Cx
# sudo ninja -Cx install
# meson x --buildtype release --strip -Db_lto=true -Dprebuilt_server=server/scrcpy-server