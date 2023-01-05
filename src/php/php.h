#define BEANMAP(clazz,var,key)\
  clazz &var=*(clazz*)getMap().get(key).asObj();

#include "Php.h"
#include "PhpCgi.h"
#include "PhpResource.h"
#include "PhpMessage.h"
#include "PhpBind.h"
#include "PhpUtil.h"
#include "PhpBean.h"
#include "PhpLogin.h"
#include "PhpSecurity.h"
#include "PhpAction.h"
#include "PhpUpload.h"
#include "PhpTag.h"
#include "PhpDat.h"
#include "PhpDao.h"
#include "PhpValidate.h"
#include "PhpAudit.h"

#include "Php.cpp"
#include "PhpAction.cpp"
#include "PhpCgi.cpp"
#include "PhpBean.cpp"
#include "PhpResource.cpp"
#include "PhpTag.cpp"
#include "PhpDat.cpp"
#include "PhpUtil.cpp"
#include "PhpMessage.cpp"
#include "PhpBind.cpp"
#include "PhpValidate.cpp"
#include "PhpUpload.cpp"
#include "PhpAudit.cpp"
#include "PhpDao.cpp"
