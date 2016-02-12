#ifndef MARSHALL_H
#define MARSHALL_H

#include <memory>
#include <vector>
#include <references.h>

class fclass;

typedef objref (marshall_fn_t)(context*,std::vector<ast*>&);
typedef objref (marshall_mthd_t)(context*,objref pThis,std::vector<ast*>&);
typedef objref (marshall_ctor_t)(context*,fclass*,std::vector<objref>&);



#endif
