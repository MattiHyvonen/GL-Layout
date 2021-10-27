#include <Layout.hpp>
#include <iostream>

void tulostaDatanKoko(Layout& L) {
  std::cout << "Datan koko tavuina: " << L.muodostaData().size() << "\n\n";
}

void tulostaRakenne(Layout& L) {
  std::cout << "Jäsenten tyypit ja muistipaikat tavuina, [alku, loppu]:\n";
  std::cout << L.haeMuotoStringina();
  std::cout << "\n";
}

/* array tarkoittaa Layoutissa yleisesti luetteloa. Käytetään std::vectoria. */
void tulostaFloatArray(const std::vector<float>& V) {
  std::cout << "{";
  for(int i=0; i<V.size(); i++) {
    std::cout << V[i];
    if(i != V.size()-1) {
      std::cout << ", ";
    }
  }
  std::cout << "}\n";
}

void tulostaFloatit(const Layout& L) {
  std::cout << "Float-tyyppiset muuttujat järjestyksessä: \n";
  for(auto f : L.haeFloatit()) {
    std::cout << f << "\n";
  }
  std::cout << "\n";
}

void tulostaFloatArrayt(const Layout& L) {
  std::cout << "Float array -tyyppiset muuttujat järjestyksessä: \n";
  std::vector<std::vector<float> > AA {L.haeFloatArrayt()};
  for(auto A : AA) {
    tulostaFloatArray(A);
  }
  std::cout << "\n";
}

int main () {
  Layout L;
  L.maaritteleJasen(float {1.0});
  L.maaritteleJasen(float {1.0});
  L.maaritteleJasen(std::vector<float> {1.0, 0.8, 0.5} );
  L.maaritteleJasen(std::vector<float> {0.5, 0.5, 0.5, 1.0, 1.0, -0.01} );
  tulostaDatanKoko(L);
  tulostaRakenne(L);
  tulostaFloatit(L);
  tulostaFloatArrayt(L);
}
