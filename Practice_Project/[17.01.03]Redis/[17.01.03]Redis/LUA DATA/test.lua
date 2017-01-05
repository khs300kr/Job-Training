local redis = require 'redis'
local client = redis.connect('127.0.0.1', 6379)
local response = client:ping()           -- true

client:set('usr:nrk', 10)
client:set('usr:nobody', 5)