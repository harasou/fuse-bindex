Vagrant.configure("2") do |config|

  config.vm.define "centos6" do |d|
    d.vm.box = "centos/6"
    d.vm.provision "shell",inline: "yum install -y fuse fuse-devel gcc"
  end

  config.vm.define "centos7" do |d|
    d.vm.box = "centos/7"
    d.vm.provision "shell",inline: "yum install -y fuse fuse-devel gcc"
  end

  config.vm.define "ubuntu14" do |d|
    d.vm.box = "ubuntu/trusty64"
    d.vm.provision "shell",inline: "apt update && apt install -y pkg-config libfuse-dev"
  end

  config.vm.define "ubuntu16" do |d|
    d.vm.box = "ubuntu/xenial64"
    d.vm.provision "shell",inline: "apt update && apt install -y pkg-config libfuse-dev"
  end


end
