// Sensör Pin Tanımlamaları


void setup() {
  
  // Motor pinlerini de burada output olarak tanımlamalısın
}

void loop() {
  // Sensörlerden veriyi oku (1: Siyah, 0: Beyaz)
  int solVal  = digitalRead(solPin);
  int ortaVal = digitalRead(ortaPin);
  int sagVal  = digitalRead(sagPin);

  if (ortaVal == 1 && solVal == 0 && sagVal == 0) {
    // Sadece orta sensör siyah görüyorsa
    duzGit();
  } 
  else if (solVal == 1) {
    // Sol sensör siyah gördüğü an (orta görse de görmese de) sola dön
    solaDon();
  } 
  else if (sagVal == 1) {
    // Sağ sensör siyah gördüğü an sağa dön
    sagaDon();
  } 
  else {
    // Hepsi beyaz görüyorsa (Çizgiden çıkıldıysa)
    dur();
  }
}

// Motor fonksiyonlarını buraya tanımlayabilirsin
void duzGit() { /* Motor ileri */ }
void solaDon() { /* Sağ motor ileri, Sol motor geri/dur */ }
void sagaDon() { /* Sol motor ileri, Sağ motor geri/dur */ }
void dur()     { /* Tüm motorlar dur */ }
