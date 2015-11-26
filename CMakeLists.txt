cmake_minimum_required(VERSION 3.1)

project(libgit2pp)
add_library(libgit2pp blob.cpp branch.cpp commit.cpp common.cpp config.cpp
  database.cpp diff.cpp exception.cpp index.cpp object.cpp oid.cpp ref.cpp
  remote.cpp repository.cpp revwalk.cpp signature.cpp status.cpp tag.cpp
  tree.cpp)
