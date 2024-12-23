resource "aws_timestreamwrite_database" "safety_socket_timestream" {
  database_name = "Safety Socket Timestream Database"
}

resource "aws_timestreamwrite_table" "device_data_table" {
  database_name = aws_timestreamwrite_database.safety_socket_timestream.database_name
  table_name    = "device_data_table"

  retention_properties {
    magnetic_store_retention_period_in_days = 30
    memory_store_retention_period_in_hours  = 8
  }

  schema {
    
  }
}
