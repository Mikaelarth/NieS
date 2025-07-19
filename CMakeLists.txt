cmake_minimum_required(VERSION 3.5)
project(NieS LANGUAGES CXX)

add_subdirectory(src)
add_subdirectory(examples/connectdb)

enable_testing()
add_subdirectory(tests)

if(DEFINED SEED_DB_NAME AND DEFINED SEED_DB_USER)
    add_custom_target(seed_db
        COMMAND sh -c "mysql -u ${SEED_DB_USER} ${SEED_DB_NAME} < ${CMAKE_SOURCE_DIR}/database/seed.sql"
        COMMENT "Load demo data into the database")
endif()
