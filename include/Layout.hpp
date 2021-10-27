#pragma once
#include <vector>
#include <string>

enum LayoutinTyyppi {
  TYYPPI_FLOAT,
  TYYPPI_ARRAY_FLOAT
};

typedef std::pair<LayoutinTyyppi, uint> LayoutinElementti;

class Layout {
  std::vector<char> data;
  std::vector<LayoutinElementti> rakenne;
  std::vector<float> muuttujat_float;
  std::vector<std::vector<float> > muuttujat_array_float;
  uint haeAlunMuistipaikka(uint jasen_i, uint alajasen_i = 0) const;
public:
  Layout(const std::vector<LayoutinElementti>& muoto);
  Layout();
  std::string haeMuotoStringina() const;
  void maaritteleJasen(const float&);
  void maaritteleJasen(const std::vector<float>&);
  const std::vector<float>& haeFloatit() const;
  const std::vector<std::vector<float> >& haeFloatArrayt() const;
  uint datanKoko() const;
  std::vector<char> muodostaData();
};
