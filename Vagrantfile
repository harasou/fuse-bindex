Vagrant.configure("2") do |config|

  config.vm.provision "shell", path: "misc/provision"

  config.vm.define "centos6" do |c|
    c.vm.box = "centos/6"
  end

  config.vm.define "centos7" do |c|
    c.vm.box = "centos/7"
  end

  config.vm.define "ubuntu1604" do |c|
    c.vm.box = "ubuntu/xenial64"
  end

  config.vm.define "ubuntu1804" do |c|
    c.vm.box = "ubuntu/bionic64"
  end

end
