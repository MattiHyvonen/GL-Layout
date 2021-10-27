#include <Layout.hpp>
#include <assert.h>
#include <iostream>

uint koko(LayoutinTyyppi T) {
  switch (T) {
    case TYYPPI_FLOAT:
    return 4;
    case TYYPPI_ARRAY_FLOAT:
    return 16;
  }
}

std::string kuvaus(LayoutinTyyppi T, uint indeksi = 0) {
  switch (T) {
    case TYYPPI_FLOAT:
    return "float";
    case TYYPPI_ARRAY_FLOAT:
    std::string indeksiStr {std::to_string(indeksi)};
    return "float array [" + indeksiStr + "]";
  }
}

uint haeSeuraavaJaollinen(uint paikka, uint jakaja) {
  uint osamaara {paikka / jakaja};
  if(osamaara*jakaja == paikka) {
    return paikka;
  }
  else {
    return (osamaara + 1)*jakaja;
  }
}

std::string muodostaRivi(uint alku, LayoutinTyyppi T, uint indeksi = 0) {
  std::string alkuStr = std::to_string(alku);
  uint loppu {alku + koko(T) - 1};
  std::string loppuStr = std::to_string(loppu);
  std::string alue {"[" + alkuStr + ", " + loppuStr + "]"};
  std::string rivi {alue + ": " + kuvaus(T, indeksi) + "\n"};
  return rivi;
}

uint Layout::haeAlunMuistipaikka(uint kohdeJasen_i, uint kohdeAlajasen_i)const {
  assert(!rakenne.empty() );
  assert(kohdeJasen_i < rakenne.size() );
  uint alunMuistipaikka {0};
  uint lopunMuistipaikka {0};
  for(int jasen_i {0}; jasen_i <= kohdeJasen_i; jasen_i++) {
    const LayoutinElementti& jasen {rakenne[jasen_i] };
    //hae jäsenen tyyppi
    const LayoutinTyyppi& jasenenTyyppi {jasen.first};
    uint alajasentenMaara {jasen.second};
    for(int alajasen_i {0}; alajasen_i < alajasentenMaara; alajasen_i++) {
      alunMuistipaikka = haeSeuraavaJaollinen(
        lopunMuistipaikka,
        koko(jasenenTyyppi)
      );
      if(jasen_i == kohdeJasen_i && alajasen_i == kohdeAlajasen_i) {
        return alunMuistipaikka;
      }
      lopunMuistipaikka = alunMuistipaikka + koko(jasenenTyyppi);
    }
  }
}

Layout::Layout(const std::vector<LayoutinElementti>& muoto) {
  rakenne = muoto;
}

Layout::Layout() {}

std::string Layout::haeMuotoStringina() const{
  std::string tulos{};
  for(uint jasen_i {0}; jasen_i < rakenne.size(); jasen_i++) {
    const LayoutinElementti& jasen {rakenne[jasen_i] };
    const LayoutinTyyppi& jasenenTyyppi {jasen.first};
    uint alajasentenMaara {jasen.second};
    for(uint alajasen_i {0}; alajasen_i < alajasentenMaara; alajasen_i++) {
      uint alunMuistipaikka {haeAlunMuistipaikka(jasen_i, alajasen_i)};
      tulos += muodostaRivi(alunMuistipaikka, jasenenTyyppi, alajasen_i);
    }
  }
  return tulos;
}

void Layout::maaritteleJasen(const float& f) {
  rakenne.push_back(LayoutinElementti{TYYPPI_FLOAT, 1} );
  muuttujat_float.push_back(f);
}

void Layout::maaritteleJasen(const std::vector<float>& V) {
  rakenne.push_back(LayoutinElementti{TYYPPI_ARRAY_FLOAT, V.size() } );
  muuttujat_array_float.push_back(V);
}

const std::vector<float>& Layout::haeFloatit() const {
  return muuttujat_float;
}

const std::vector<std::vector<float> >& Layout::haeFloatArrayt() const {
  return muuttujat_array_float;
}

uint Layout::datanKoko() const {
  uint viimeinenJasen_i {(uint)rakenne.size()-1};
  LayoutinElementti viimeinenJasen {rakenne[viimeinenJasen_i]};
  LayoutinTyyppi viimeisenTyyppi {viimeinenJasen.first};
  uint viimeisenMaara {viimeinenJasen.second};
  uint viimeisenViimeisenAlku {
    haeAlunMuistipaikka(viimeinenJasen_i, viimeisenMaara-1)
  };
  uint loppu {viimeisenViimeisenAlku + koko(viimeisenTyyppi) };
  return loppu;
}

std::vector<char> Layout::muodostaData() {
  data.resize(datanKoko());
  uint float_i {0};
  uint array_i {0};
  for(int jasen_i {0}; jasen_i < rakenne.size(); jasen_i++) {
    LayoutinElementti jasen {rakenne[jasen_i]};
    LayoutinTyyppi jasenenTyyppi {jasen.first};
    uint alajasentenMaara {jasen.second};
    for(uint alajasen_i {0}; alajasen_i < alajasentenMaara; alajasen_i++) {
      uint alunMuistipaikka {haeAlunMuistipaikka(jasen_i, alajasen_i)};
      char* alunOsoite {&(data[alunMuistipaikka]) };
      switch (jasenenTyyppi) {
        case TYYPPI_FLOAT:
        assert(float_i < muuttujat_float.size() );
        *((float*)alunOsoite) = muuttujat_float[float_i];
        float_i++;
        break;
        case TYYPPI_ARRAY_FLOAT:
        assert(array_i < muuttujat_array_float.size() );
        const std::vector<float>& fArray {muuttujat_array_float[array_i]};
        *((float*)alunOsoite) = fArray[alajasen_i];
        break;
      }
    }
    switch (jasenenTyyppi) {
      case TYYPPI_ARRAY_FLOAT:
      array_i++;
      break;
    }
  }
  return data;
}
