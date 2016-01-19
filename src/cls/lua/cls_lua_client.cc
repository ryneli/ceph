#include <errno.h>
#include <string>
#include <vector>
#include "include/encoding.h"
#include "include/rados.h"
#include "include/rados/librados.h"
#include "include/types.h"
#include "cls_lua.h"
#include "cls_lua_client.hpp"

using std::string;
using std::vector;
using librados::IoCtx;
using librados::bufferlist;

namespace cls_lua_client {

  int exec(IoCtx& ioctx, const string& oid, const string& script,
      const string& handler, bufferlist& input, bufferlist& output)
  {
    int ret;

    bufferlist inbl;

    ::encode(script, inbl);
    ::encode(handler, inbl);
    ::encode(input, inbl);

    /*
     * TODO: we need to encapsulate the return value as well. for example,
     * -ENOTSUPP is returned if the class is not found, but we also return
     * -ENOTSUPP if a handler isn't found. In the later case we still get a
     * valid reply, in the former not so much.
     */
    ret = ioctx.exec(oid, "lua", "eval_bufferlist", inbl, output);

    return ret;
  }

}
