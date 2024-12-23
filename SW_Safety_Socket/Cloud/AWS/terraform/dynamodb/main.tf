resource "aws_dynamodb_table" "safety_socket_dynamodb_user_auth_table" {
  name           = "user_auth_table"
  billing_mode   = "PAY_PER_REQUEST"
  read_capacity  = 20
  write_capacity = 20
  hash_key       = "userid"
  range_key      = "username"

  attribute {
    name = "userid"
    type = "S"
  }

  attribute {
    name = "username"
    type = "S"
  }

  attribute {
    name = "password"
    type = "S"
  }

  attribute {
    name = "private_key"
    type = "S"
  }

  attribute {
    name = "publish_key"
    type = "S"
  }

  attribute {
    name = "level"
    type = "N"
  }

  ttl {
    attribute_name = "TimeToExist"
    enabled        = false
  }
}

resource "aws_dynamodb_table" "safety_socket_dynamodb_device_credential_table" {
  name           = "device_credential_table"
  billing_mode   = "PAY_PER_REQUEST"
  read_capacity  = 20
  write_capacity = 20
  hash_key       = "deviceid"
  range_key      = "userid"

  attribute {
    name = "deviceid"
    type = "S"
  }

  attribute {
    name = "userid"
    type = "S"
  }

  attribute {
    name = "device_private"
    type = "S"
  }

  attribute {
    name = "name"
    type = "S"
  }

  attribute {
    name = "meshid"
    type = "S"
  }

  attribute {
    name = "meshpass"
    type = "S"
  }
}

resource "aws_dynamodb_table" "safety_socket_dynamodb_device_scene_table" {
  name           = "device_scene_table"
  billing_mode   = "PAY_PER_REQUEST"
  read_capacity  = 20
  write_capacity = 20
  hash_key       = "id"
  range_key      = "deviceid"

  attribute {
    name = "id"
    type = "S"
  }

  attribute {
    name = "deviceid"
    type = "S"
  }

  attribute {
    name = "data"
    type = "S"
  }

  attribute {
    name = "exp"
    type = "N"
  }
}