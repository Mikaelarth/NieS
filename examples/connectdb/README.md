# ConnectDB Example

This small program demonstrates how to use `QSqlDatabase` to connect to a PostgreSQL server, run a simple query and report errors.

## Run in Qt Creator

1. Open **File > Open File or Project...** and select `examples/connectdb/CMakeLists.txt`.
2. Configure a build directory when prompted. Make sure Qt's SQL module is available.
3. `DatabaseManager` searches for `config.ini` in the executable's directory. Copy
   the file there or set `NIES_CONFIG_PATH` to point to a custom location.
4. Build and run the `connectdb` target. The Application Output pane will show either the PostgreSQL version string or an error message.
