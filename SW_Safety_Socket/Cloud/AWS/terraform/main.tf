# Set up Terraform provider version
terraform {
  required_providers {
    aws = {
      source  = "hashicorp/aws"
      version = "~>4.29"
    }
  }
}

# Defining AWS provider
provider "aws" {
  region = "us-east-1"
}

# Create DynamoDB
module "safety_socket_dynamodb" {
  source = "./dynamodb"
}
