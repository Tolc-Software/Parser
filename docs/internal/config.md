# Config interface #

To externally affect the behaviour of the `Parser`, you can use the `Config` object. There you can set things like `log level` etc. and it should have sane defaults.

## Goal ##

The `Config` object should:

1. Be default constructable
2. When default constructed, give sane defaults without uninitialized variables
3. Contain **no** logic, functions, or private member variables
