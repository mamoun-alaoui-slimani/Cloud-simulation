#pragma once

class Montagne;
class Ciel;


class SupportADessin
{
 public:
    /**
  virtual ~SupportADessin() = default;
  // on ne copie pas les Supports
  SupportADessin(SupportADessin const&)            = delete; 
  SupportADessin& operator=(SupportADessin const&) = delete;
   // mais on peut les déplacer
  SupportADessin(SupportADessin&&)            = default; 
  SupportADessin& operator=(SupportADessin&&) = default;
  
  SupportADessin() = default; 
  **/
  virtual ~SupportADessin() = default;
  virtual void dessine(Montagne const& montagne) = 0;
  virtual void dessine(Ciel const& ciel) = 0;
};
