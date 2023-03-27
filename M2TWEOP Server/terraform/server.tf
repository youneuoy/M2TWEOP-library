# Region our resources will be created in
provider "aws" {
  region  = "eu-west-1"
}

# The server's OS/AMI (Amazon Machine Image)
data "aws_ami" "ubuntu" {
    most_recent = true
    filter {
            name   = "name"
            values = ["ubuntu/images/hvm-ssd/*20.04-amd64-server-*"]
        }
    filter {
            name   = "virtualization-type"
            values = ["hvm"]
        }
    owners = ["099720109477"]
}

# The actual server itself
resource "aws_instance" "m2tweop_server" {
  ami           = data.aws_ami.ubuntu.id
  instance_type = "t3.micro"
  key_name      = "m2tweop-server-ssh-key"
  tags = {
    Name = var.ec2_name
  }
}