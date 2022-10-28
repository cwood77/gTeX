#pragma once

class iMapAccessorPass {
public:
   virtual std::string getMap(const std::string& id) = 0;
};
