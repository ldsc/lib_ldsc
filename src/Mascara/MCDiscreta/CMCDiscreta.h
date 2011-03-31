//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined CMCDiscreta_h
#define CMCDiscreta_h

/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
		Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades 
Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMCDiscreta.h
Nome da classe:      CMCDiscreta
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
//  ----------------------------------------------------------------------------
//  Classe:       CMCDiscreta
//  ----------------------------------------------------------------------------
/**
 * @brief Implementa conceito de mascara de chanfro discreta.
 * 
 * Implementa a classe CMCDiscreta.  E' derivada da classe TMascara.
 * Acrescenta o conceito de mascara discreta.
 * Acrescenta atributos raioBolaInclusa e raioBolaTangente
 * .
 * Representa uma mascara de chanfro. 
 */
class CMCDiscreta	
{
//  Atributos novos
public:
  int raioBase;	///<  raio utilizado pelas mascaras para definir o raio da bola.
  /// RDF=raioBase*mBase +
  
protected:		

//  Metodos
public:
   	///  Construtor, tamanho da mascara
 CMCDiscreta (int rb):raioBase (rb)
  {
  }
	///  Destrutor
  virtual ~ CMCDiscreta ()
  {
  }

public:
	/// Seta o raio base
  void RaioBase (int rb)
  {
    raioBase = rb;
  }
  
/// Retorna o raio
  int RaioBase () const
  {
    return raioBase;
  }

  //  virtual CalculaRaioBase();
};
#endif //  TMascMascDiscreta
