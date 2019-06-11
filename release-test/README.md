# Project 1: Learn Bazel

## Install Bazel 0.13

## Files You Need to Change

third-party-libs/rocksdb/BUILD

Modify the above file to add or remove some lines from it to make the rocksdb related test successfully pass.

## Command You Need to Run

```
bazel test experiments:rocksdb_test --test_output all
```

The above command need to print PASS to complete this project.

## Platforms

The only supported platforms are Linux and MacOS.
