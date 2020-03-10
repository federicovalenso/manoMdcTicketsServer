unix {
  DESTDIR = /usr/bin/
  ETCDIR = /var/www
}
win32 {
  DESTDIR = $$OUT_PWD/bin
  ETCDIR = $$DESTDIR/etc
}

ASSETSDIR = $$_PRO_FILE_PWD_/src/etc
webpack_commands = cd
win32 {
  webpack_commands += /d
}
webpack_commands += $$ASSETSDIR && npm install && npm run
debug {
  webpack_commands += dev
} else {
  webpack_commands += prod
}
webpack_commands += && $(COPY_DIR) \"$$ASSETSDIR/docroot\" \"$$ETCDIR/\"
webpack_commands += && $(COPY_DIR) \"$$ASSETSDIR/templates\" \"$$ETCDIR/\"
webpack.target = webpack
webpack.commands = $$webpack_commands

QMAKE_EXTRA_TARGETS += webpack
