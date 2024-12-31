var userModelData = {
  "_email" : "",
  "_id" : "",
  "_name" : "",
  "_token" : "",
};

Map<String, String> getHeader() {
  return {
    'Accept': 'application/json',
    "ACCESS_TOKEN": userModelData['_token'].toString(),
    "CLIENT_ID": userModelData['_id'].toString()
  };
}