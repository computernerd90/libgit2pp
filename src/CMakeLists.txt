cmake_minimum_required(VERSION 3.4)

include_directories("../libgit2/include")

add_library(git2pp blob.cpp branch.cpp commit.cpp common.cpp config.cpp
  database.cpp diff.cpp exception.cpp index.cpp object.cpp oid.cpp ref.cpp
  remote.cpp repository.cpp revwalk.cpp signature.cpp status.cpp tag.cpp
  tree.cpp)

set_property(TARGET git2pp PROPERTY CXX_STANDARD 14)
