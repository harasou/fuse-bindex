Fuse-BindEx
=============================================
bindex is a file system like bind mount using fuse. When you read the executable
file, the execution result is read.

[![][Travis]][Travis-Link] [![][MIT]][MIT-Link]

Requirements
--------------------------------------------------------------------------------
- Linux kernel supporting FUSE
- FUSE 2.6 (or later) package
  - libfuse2 (Ubuntsu/debian)
  - fuse-libs (CentOS)

Install
--------------------------------------------------------------------------------
- Download rpm or deb package from [GitHub Releases][]. and install package.

Or.

- Build on linux

```
git clone https://github.com/harasou/fuse-bindex.git
cd fuse-bindex

sudo misc/provision
make
bin/bindex --help
```


Usage
--------------------------------------------------------------------------------

1. Create mount point

    ```
    mkdir /mnt/bindex
    ```

1. Preparation of executable file

    ```
    mkdir /opt/bindex/
    cat <<EOD> /opt/bindex/now
    #!/bin/bash
    date "+%F %T"
    EOD
    chmod +x /opt/bindex/now
    ```

1. Mount

    ```
    bindex /opt/bindex /mnt/bindex
    mount -t fuse.bindex
    ```

1. Done

    ```
    cat /mnt/bindex/now
    ```
    ex.
    ```
    root@ubuntu-bionic:~/fuse-bindex# cat /mnt/bindex/now
    2018-12-31 13:26:25
    root@ubuntu-bionic:~/fuse-bindex# cat /mnt/bindex/now
    2018-12-31 13:26:28
    root@ubuntu-bionic:~/fuse-bindex# cat /mnt/bindex/now
    2018-12-31 13:26:31
    ```

License
--------------------------------------------------------------------------------
Paddington is licensed under the [MIT][MIT-Link] license.
Copyright (c) 2018 SOGO Haraguchi


<!-- links -->
[Travis]: https://img.shields.io/travis/harasou/fuse-bindex.svg?style=flat-square
[Travis-Link]: https://travis-ci.org/harasou/fuse-bindex
[MIT]: https://img.shields.io/github/license/mashape/apistatus.svg?style=flat-square
[MIT-Link]: https://github.com/harasou/exbind-fuse/blob/master/LICENSE
[GitHub Releases]: https://github.com/harasou/fuse-bindex/releases
