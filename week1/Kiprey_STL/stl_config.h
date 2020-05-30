#pragma once

#define __STL_NAME Kiprey_STL
#define __STL_BEGIN_NAMESPACE namespace __STL_NAME{
#define __STL_END_NAMESPACE }

#define __STL_TRY try
#define __STL_UNWIND(action) catch(...){action; throw;}

#define __STL_TEMPLATE_NULL template<>