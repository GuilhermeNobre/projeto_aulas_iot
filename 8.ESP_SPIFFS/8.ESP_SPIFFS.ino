/******************************************************************************
                  SPIFFS: Armazenamento de Arquivos do ESP32
                              Sketch Principal

                            Criado em 11 mar. 2022
                              por Michel Galvão
******************************************************************************/

// Inclusão da(s) biblioteca(s)
#include "SPIFFS.h"
#include "FS.h"

// Protótipo das Funções
String readStringSerial();
char readCharSerial();
bool listDir();
bool readFile(String path);
bool writeFile(String path, String message);
bool appendFile(String path, String message);
bool renameFile(String path1, String path2);
bool deleteFile(String path);
bool moveFile(String pathOrigem, String pathDestino);
bool copyFile(String pathOrigem, String pathDestino);
bool formatSPIFFS();

void setup() { // Configurações Iniciais
  Serial.begin(115200); // Inicializa Serial
  delay(1000);

  // Splash Screen na Serial
  Serial.println("\n\n");
  Serial.println("  -> SPIFFS: Armazenamento de Arquivos do ESP32 <-  ");
  if (SPIFFS.begin(false)) // Inicializa a partição SPIFFS, sem formata-la
  {
    Serial.println("SPIFFS inicializado!");
  }
  else {
    Serial.println("Falha na inicialização SPIFFS");
    Serial.println("[Programa terminado]");
    while (1); // Loop infinito
  }
}

void loop() { // Loop Infinito
  char opcao; // armazena a opção do menu selecionada pelo usuário

  // Mostra as opções do menu na Serial
  Serial.println("MENU");
  Serial.println("Digite a opção desejada e clique em ENTER:");
  Serial.println("  1. Ler Arquivo");
  Serial.println("  2. Novo Arquivo");
  Serial.println("  3. Editar Arquivo (adicionar Conteúdo)");
  Serial.println("  4. Renomear Arquivo");
  Serial.println("  5. Excluir Arquivo  ");
  Serial.println("  6. Copiar Arquivo para...");
  Serial.println("  7. Mover Arquivo para...");
  Serial.println("  8. Formatar SPIFFS  ");

  opcao = readCharSerial(); // lê a opção selecionada pelo usuáro na Serial e armazena
  Serial.print("opcao ");
  Serial.print(opcao);
  Serial.println(" selecionada");

  switch (opcao) { // exibe determinada opção de acordo com a seleção do usuário
    case '1': //  Ler Arquivo
      {
        Serial.print("-> Para sair digite SAIR, ou ");
        Serial.println("Diante dos seguintes arquivos, ");
        if (!listDir()) //lista os arquivos do "diretório" '/', se houver
          //                  alguma falha é cancelada a operação.
        {
          break;
        }


        Serial.print("-> digite o caminho e nome do arquivo à ser lido ");
        Serial.print("(Exemplo: /arquivo.txt): ");
        String path = readStringSerial(); //  Espera e lê da Serial a entrada
        //                                      do usuário para o caminho do arquivo
        //                                      à ser lido.
        if (path == "SAIR") // se a entrada do usuário for "SAIR", a operação é
          //                      cancelada.
        {
          Serial.println("\nSAINDO");
          break;
        }
        Serial.println(path);

        Serial.println("Conteúdo do Arquivo: ");
        Serial.println("-------------------------------------------");
        if (readFile(path)) { // Lê o conteúdo do arquivo e mostra na Serial.
          Serial.println("-------------------------------------------");
          Serial.println();
        }
        break;
      }
    case '2': // Novo Arquivo
      {
        Serial.print("-> Para sair digite SAIR, ou ");
        Serial.print("-> Digite o caminho e nome do arquivo à ser gravado ");
        Serial.print("(Exemplo: /arquivo.txt): ");
        String path = readStringSerial(); //  Espera e lê da Serial a entrada
        //                                      do usuário para o caminho do arquivo
        //                                      à ser gravado.
        if (path == "SAIR") // se a entrada do usuário for "SAIR", a operação é
          //                      cancelada.
        {
          Serial.println("\nSAINDO");
          break;
        }
        Serial.println(path);

        Serial.println("Digite o conteúdo do arquivo à ser gravado: ");
        Serial.println("INSTRUÇÕES:");
        Serial.print("  - Digite o conteúdo linha a linha e clique em ENTER após ");
        Serial.println("cada inserção de linha;");
        Serial.println("  - Digite VER para visualizar o conteúdo já escrito;");
        Serial.print("  - Digite SALVAR para terminar de inserir conteúdo e");
        Serial.println("salva - lo; ");
        Serial.print("  - Digite LIMPAR para limpar todo conteúdo inserido e ");
        Serial.println("começar a inserção novamente; ");
        Serial.print("  - Digite SAIR para limpar todo conteúdo inserido e ");
        Serial.println("sair sem salvar.");
        String message; // variável que armazenará o texto à ser gravado na SPIFFS
        bool controleWhile = true; // controla se deve ou não permanecer no while
        while (controleWhile) { // estrutura de repetição que controla e
          //                        manipula a inserção de dados pelo usuário.
          String line = readStringSerial(); // lê da Serial a nova linha
          //                                    para armazenar no arquivo.
          if (line == "SALVAR") { // se o comando recebido for SALVAR, ...
            Serial.println();
            Serial.println("-> SALVANDO");
            writeFile(path, message); // Escreve o arquivo, com os dados
            //                            inseridos pelo usuário,
            //                            no caminho estabelecido pelo usuário.
            controleWhile = false; // sai da estrutura de repetição
          }
          else if (line == "LIMPAR") { // se o comando recebido for LIMPAR, ...
            Serial.println();
            Serial.println("-> LIMPANDO");
            message = ""; // limpa todo o texto inserido pelo usuário
          }
          else if (line == "SAIR") { // se o comando recebido for SAIR, ...
            Serial.println("-> SAINDO");
            controleWhile = false; //  sai da estrutura de repetição
            //                          sem salvar nenhum arquivo.
          }
          else if (line == "VER") { // se o comando recebido for VER, ...
            Serial.println();
            Serial.print(message); // mostra na Serial todo o texto já inserido
          }
          else { // se nenhum comando pré-estabelecido for encontrado, ...
            Serial.print("-> ");
            Serial.println(line);
            message += line; // adiciona a linha inserida pelo usuário
            //                    na váriavel de armazenamneto de todo
            //                    o texto do novo arquivo.
            message += "\r\n"; // adiciona os caracteres de retorno
            //                      de carro e de nova linha.
          }
        }
        break;
      }
    case '3': // Editar Arquivo (adicionar Conteúdo)
      {
        Serial.print("-> Para sair digite SAIR, ou ");
        Serial.print("-> Digite o caminho e nome do arquivo à ser editado ");
        Serial.print("(Exemplo: / arquivo.txt): ");
        String path = readStringSerial(); //  Espera e lê da Serial a entrada
        //                                      do usuário para o caminho do arquivo
        //                                      à ser lido.
        if (path == "SAIR")  // se a entrada do usuário for "SAIR", a operação é
          //                      cancelada.
        {
          Serial.println("\nSAINDO");
          break;
        }
        Serial.println(path);


        if (!SPIFFS.exists(path)) // verfica se um arquivo com o
          //                            caminho fornecido não existe.
        {
          Serial.println(" - falha ao abrir arquivo para edição.");
          break;
        }
        Serial.println("Digite o conteúdo do arquivo à ser adicionado: ");
        Serial.println("INSTRUÇÕES: ");
        Serial.print("  - Digite o conteúdo linha a linha e clique em ENTER após ");
        Serial.println("cada inserção de linha; ");
        Serial.print("  - Digite VER_ARQUIVO para visualizar o arquivo já ");
        Serial.println("escrito; ");
        Serial.println("  - Digite VER para visualizar o conteúdo já escrito; ");
        Serial.print("  - Digite SALVAR para terminar de inserir conteúdo ");
        Serial.println("e salva - lo; ");
        Serial.print("  - Digite LIMPAR para limpar todo conteúdo inserido e ");
        Serial.println("começar a inserção novamente; ");
        Serial.println("  - Digite SAIR para limpar todo conteúdo inserido e sair "
                       "sem salvar.");
        String message; // variável que armazenará o texto à ser gravado na SPIFFS
        bool controleWhile = true; // controla se deve ou não permanecer no while
        while (controleWhile)  // estrutura de repetição que controla e
          //                        manipula a inserção de dados pelo usuário.
        {
          String line = readStringSerial(); // lê da Serial a nova linha
          //                                    para anexar ao arquivo.
          if (line == "SALVAR") { // se o comando recebido for SALVAR, ...
            Serial.println();
            Serial.println("-> SALVANDO");
            appendFile(path, message);//Anexa ao arquivo os dados
            //                           inseridos pelo usuário,
            //                           no caminho/arquivo estabelecido pelo usuário.
            controleWhile = false; // sai da estrutura de repetição
          }
          else if (line == "LIMPAR") { // se o comando recebido for LIMPAR, ...
            Serial.println();
            Serial.println("-> LIMPANDO");
            message = ""; // limpa todo o texto inserido pelo usuário
          }
          else if (line == "SAIR") { // se o comando recebido for SAIR, ...
            Serial.println("-> SAINDO");
            controleWhile = false; //  sai da estrutura de repetição
            //                          sem salvar nenhum arquivo.
          }
          else if (line == "VER_ARQUIVO") //se o comando recebido for VER_ARQUIVO, ...
          {
            Serial.println();
            Serial.println("-------------------------------------------");
            if (readFile(path)) { // mostra na Serial o texto do arquivo já salvo,
              //                      no caminho especifcado pelo usuário.
              Serial.println("-------------------------------------------");
              Serial.println();
            }

          }
          else if (line == "VER") { // se o comando recebido for VER, ...
            Serial.println();
            Serial.print(message); // mostra na Serial todo o texto já inserido
          } else { // se nenhum comando pré-estabelecido for encontrado, ...
            Serial.print("-> ");
            Serial.println(line);
            message += line; // adiciona a linha inserida pelo usuário
            //                    na váriavel de armazenamneto de todo
            //                    o texto para adicionar no arquivo.
            message += "\r\n"; // adiciona os caracteres de retorno
            //                      de carro e de nova linha.
          }
        }
        break;
      }
    case '4': // Renomear Arquivo
      {
        Serial.print("-> Para sair digite SAIR, ou ");
        Serial.print("-> Digite o caminho e nome do arquivo à ser renomeado ");
        Serial.print("(Exemplo: / arquivo.txt): ");
        String path1 = readStringSerial(); //  Espera e lê da Serial a entrada
        //                                      do usuário para o caminho do arquivo
        //                                      à ser renomeado.
        if (path1 == "SAIR") // se a entrada do usuário for "SAIR", a operação é
          //                      cancelada.
        {
          Serial.println("\nSAINDO");
          break;
        }
        Serial.println(path1);

        Serial.print("-> Para sair digite SAIR, ou ");
        Serial.print("Digite o caminho e nome do arquivo para renomeação ");
        Serial.print("(Exemplo: / arquivoRenomeado.txt): ");
        String path2 = readStringSerial(); //  Espera e lê da Serial a entrada
        //                                      do usuário para o novo nome do
        //                                      caminho do arquivo à ser renomeado.
        if (path2 == "SAIR") // se a entrada do usuário for "SAIR", a operação é
          //                      cancelada.
        {
          Serial.println("\nSAINDO");
          break;
        }
        Serial.println(path2);

        Serial.print("-> Deseja realmente renomear o arquivo ");
        Serial.print(path1);
        Serial.print(" para ");
        Serial.print(path2);
        Serial.print(" ? Digite SIM ou NÃO : ");
        String resposta = readStringSerial(); // lê da Serial a confirmação se o
        //                                        usuário quer realmente renomear
        //                                        o arquivo.
        Serial.println(resposta);
        if (resposta == "SIM") { // se a resposta do usuário for SIM, ...
          renameFile(path1, path2); // Renomeia o arquivo de nome
          //                            antigo 'path1' para o novo nome 'path2'.
        }
        else if (resposta == "NÃO") { // se não, se a resposta do usuário for NÃO, ...
          Serial.println("O arquivo NÃO foi renomeado."); // Mostra ao usuário que
          //                                                  o arquivo não foi
          //                                                  renomeado.
        }
        else { // se nenhum comando pré-estabelecido for encontrado, ...
          Serial.println("Opção inválida. Tente novamente."); // Mostra que a Opção
          //                                                      é inválida.
        }
        break;
      }
    case '5': // Excluir Arquivo
      {
        Serial.print("-> Para sair digite SAIR, ou ");
        Serial.print("-> Digite o caminho e nome do arquivo à ser excluído ");
        Serial.print("(Exemplo: / arquivo.txt) : ");
        String path = readStringSerial(); //  Espera e lê da Serial a entrada
        //                                      do usuário para o caminho do arquivo
        //                                      à ser excluído.
        if (path == "SAIR") // se a entrada do usuário for "SAIR", a operação é
          //                      cancelada.
        {
          Serial.println("\nSAINDO");
          break;
        }
        Serial.println(path);

        Serial.print("Deseja realmente excluir o arquivo ");
        Serial.print(path);
        Serial.print(" ? Esta é uma operação não reversível e permanente. ");
        Serial.print("-> Digite SIM ou NÃO : ");
        String resposta = readStringSerial(); // lê da Serial a confirmação se o
        //                                        usuário quer realmente renomear
        //                                        o arquivo.
        Serial.println(resposta);
        if (resposta == "SIM") { // se a resposta do usuário for SIM, ...
          deleteFile(path); // exclui o arquivo do caminho selecionado pelo usuário
        }
        else if (resposta == "NÃO") { // se não, se a resposta do usuário for NÃO, ...
          Serial.println("O arquivo NÃO foi excluído."); // Mostra ao usuário que
          //                                                  o arquivo não foi
          //                                                  excluído.
        }
        else { // se nenhum comando pré-estabelecido for encontrado, ...
          Serial.println("Opção inválida. Tente novamente."); // Mostra que a Opção
          //                                                      é inválida.
        }
        break;
      }
    case '6': // Copiar Arquivo para...
      {
        Serial.print("-> Para sair digite SAIR, ou ");
        Serial.print("-> Digite o caminho e nome do arquivo de ORIGEM para cópia ");
        Serial.print("(Exemplo: / arquivo.txt) : ");
        String pathOrigem = readStringSerial(); //  Espera e lê da Serial a entrada
        //                                      do usuário para o caminho do arquivo
        //                                      à ser copiado.
        if (pathOrigem == "SAIR") // se a entrada do usuário for "SAIR", a operação é
          //                      cancelada.
        {
          Serial.println("\nSAINDO");
          break;
        }
        Serial.println(pathOrigem);

        Serial.print("-> Para sair digite SAIR, ou ");
        Serial.print("-> Digite o caminho e nome do arquivo de DESTINO para cópia ");
        Serial.print("(Exemplo: / arquivo.txt) : ");
        String pathDestino = readStringSerial(); // Espera e lê da Serial a entrada
        //                                            do usuário para o novo caminho
        //                                            do arquivo à ser copiado.
        if (pathDestino == "SAIR") // se a entrada do usuário for "SAIR", a operação é
          //                            cancelada.
        {
          Serial.println("\nSAINDO");
          break;
        }
        Serial.println(pathDestino);

        Serial.print("Deseja realmente copiar o arquivo ");
        Serial.print(pathOrigem);
        Serial.print(" para ");
        Serial.print(pathDestino);
        Serial.print(" ? ");
        Serial.print("-> Digite SIM ou NÃO : ");
        String resposta = readStringSerial(); // lê da Serial a confirmação se o
        //                                        usuário quer realmente copiar
        //                                        o arquivo.
        Serial.println(resposta);
        if (resposta == "SIM") { // se a resposta do usuário for SIM, ...
          copyFile(pathOrigem, pathDestino); // copia o arquivo do caminho
          //                                      de origem para o de destino,
          //                                      ambos selecionados pelo usuário.
        }
        else if (resposta == "NÃO") { // se não, se a resposta do usuário for NÃO, ...
          Serial.println("O arquivo NÃO foi copiado."); // Mostra ao usuário que
          //                                                  o arquivo não foi
          //                                                  copiado.
        }
        else { // se nenhum comando pré-estabelecido for encontrado, ...
          Serial.println("Opção inválida. Tente novamente."); // Mostra que a Opção
          //                                                      é inválida.
        }
        break;
      }
    case '7': // Mover Arquivo para...
      {
        Serial.print("-> Para sair digite SAIR, ou ");
        Serial.print("-> Digite o caminho e nome do arquivo de ORIGEM para mover ");
        Serial.print("(Exemplo: / arquivo.txt) : ");
        String pathOrigem = readStringSerial(); //  Espera e lê da Serial a entrada
        //                                      do usuário para o caminho do arquivo
        //                                      à ser movido.
        if (pathOrigem == "SAIR")  // se a entrada do usuário for "SAIR", a operação é
          //                            cancelada.
        {
          Serial.println("\nSAINDO");
          break;
        }
        Serial.println(pathOrigem);

        Serial.print("-> Para sair digite SAIR, ou ");
        Serial.print("-> Digite o caminho e nome do arquivo de DESTINO para mover ");
        Serial.print("(Exemplo: / arquivo.txt) : ");
        String pathDestino = readStringSerial(); // Espera e lê da Serial a entrada
        //                                            do usuário para o novo caminho
        //                                            do arquivo à ser movido.
        if (pathDestino == "SAIR") // se a entrada do usuário for "SAIR", a operação é
          //                            cancelada.
        {
          Serial.println("\nSAINDO");
          break;
        }
        Serial.println(pathDestino);

        Serial.print("Deseja realmente mover o arquivo ");
        Serial.print(pathOrigem);
        Serial.print(" para ");
        Serial.print(pathDestino);
        Serial.print(" ? ");
        Serial.print("-> Digite SIM ou NÃO : ");
        String resposta = readStringSerial(); // lê da Serial a confirmação se o
        //                                        usuário quer realmente mover
        //                                        o arquivo.
        Serial.println(resposta);
        if (resposta == "SIM") { // se a resposta do usuário for SIM, ...
          moveFile(pathOrigem, pathDestino); // move o arquivo do caminho
          //                                      de origem para o de destino,
          //                                      ambos selecionados pelo usuário.
        }
        else if (resposta == "NÃO") { // se não, se a resposta do usuário for NÃO, ...
          Serial.println("O arquivo NÃO foi movido."); // Mostra ao usuário que
          //                                                  o arquivo não foi
          //                                                  movido.
        }
        else { // se nenhum comando pré-estabelecido for encontrado, ...
          Serial.println("Opção inválida. Tente novamente."); // Mostra que a Opção
          //                                                      é inválida.
        }
        break;
      }
    case '8': // Formatar SPIFFS
      {
        Serial.println("-> Para sair digite SAIR, ou siga as instruções seguintes.");
        Serial.println("ATENÇÃO");
        Serial.print("  Esta opção permite a formatação da SPIFFS. Este ");
        Serial.println(" procedimento de formatação apagará tudo.");
        Serial.print("  Se você tiver algum arquivo importante em seu sistema ");
        Serial.println(" de arquivos, não siga esta opção ");
        Serial.println("  antes de salvá -lo em outro local.");
        Serial.print("-> Deseja realmente seguir com esta opção ? Digite SIM ");
        Serial.println("ou NÃO : ");
        String resposta = readStringSerial(); // lê da Serial a confirmação se o
        //                                        usuário quer realmente formatar
        //                                        a partição SPIFFS.
        if (resposta == "SAIR") // se a entrada do usuário for "SAIR", a operação é
          //                          cancelada.
        {
          Serial.println("\nSAINDO");
          break;
        }
        Serial.println(resposta);
        if (resposta == "SIM") { // se a resposta do usuário for SIM, ...
          Serial.println("Formatando SPIFFS");
          formatSPIFFS(); // formata a partição SPIFFS
        } else if (resposta == "NÃO") {//se não, se a resposta do usuário for NÃO, ...
          Serial.println("SPIFFS não será formatada"); // Mostra ao usuário que
          //                                                a partição SPIFFS
          //                                                não foi formatada.
        } else { // se nenhum comando pré-estabelecido for encontrado, ...
          Serial.println("Opção inválida. Tente novamente."); // Mostra que a Opção
          //                                                      é inválida.
        }
        break;
      }
    default: // Opção selecionada fora dos valores padrão
      Serial.println("Opção Inválida"); // Mostra que a Opção
      //                                    é inválida.
      break;
  }

  delay(1000);
  Serial.println();
}

/**
  Espera o usuário da Serial digitar e enviar algum texto.

  @return o texto digitado no tipo String.
*/
String readStringSerial() {
  String retorno; // String de retorno
  bool controleLeitura = true; // controle do while
  while (controleLeitura) { // espera o usuário digitar algo na Serial
    delay(10);
    if (Serial.available()) { // se houver algo na Serial para ler, ...
      retorno = Serial.readString(); // lê os caracteres do buffer serial em
      //                                  uma String.
      controleLeitura = false; // define a variável de controle do while
      //                            para sair da repetição.
    }
  }
  return retorno; // retorna a String capturada
}

/**
  Espera o usuário da Serial digitar e enviar algum caractere.

  @return o caractere digitado no tipo char.
*/
char readCharSerial() {
  char retorno; // caractere de retorno
  bool controleLeitura = true; // controle do while
  while (controleLeitura) { // espera o usuário digitar algo na Serial
    delay(10);
    if (Serial.available()) { // se houver algo na Serial para ler, ...
      retorno = Serial.read(); // lê o caractere da serial
      controleLeitura = false; // define a variável de controle do while
      //                            para sair da repetição.
    }
  }
  return retorno; // retorna o caractere capturado
}

/**
  Lista todos os arquivos da SPIFFS

  @return true se ocorreu tudo certo ou false se houve algum erro
*/
bool listDir() {
  File root = SPIFFS.open("/"); // Abre o "diretório" onde estão os arquivos na SPIFFS
  //                                e passa o retorno para
  //                                uma variável do tipo File.
  if (!root) // Se houver falha ao abrir o "diretório", ...
  {
    // informa ao usuário que houve falhas e sai da função retornando false.
    Serial.println(" - falha ao abrir o diretório");
    return false;
  }

  File file = root.openNextFile(); // Relata o próximo arquivo do "diretório" e
  //                                    passa o retorno para a variável
  //                                    do tipo File.
  int qtdFiles = 0; // variável que armazena a quantidade de arquivos que
  //                    há no diretório informado.
  while (file) { // Enquanto houver arquivos no "diretório" que não foram vistos,
    //                executa o laço de repetição.
    Serial.print("  FILE : ");
    Serial.print(file.name()); // Imprime o nome do arquivo
    Serial.print("\tSIZE : ");
    Serial.println(file.size()); // Imprime o tamanho do arquivo
    qtdFiles++; // Incrementa a variável de quantidade de arquivos
    file = root.openNextFile(); // Relata o próximo arquivo do diretório e
    //                              passa o retorno para a variável
    //                              do tipo File.
  }
  if (qtdFiles == 0)  // Se após a visualização de todos os arquivos do diretório
    //                      não houver algum arquivo, ...
  {
    // Avisa o usuário que não houve nenhum arquivo para ler e retorna false.
    Serial.print(" - Sem arquivos para ler. Crie novos arquivos pelo menu ");
    Serial.println("principal, opção 2.");
    return false;
  }

  return true; // retorna true se não houver nenhum erro
}

/**
  Lê o conteúdo de um arquivo especificado da SPIFFS

  @param path : o caminho/nome do arquivo à ser listado
  @return true se ocorreu tudo certo ou false se houve algum erro
*/
bool readFile(String path) {
  File file = SPIFFS.open(path); // Abre o caminho do arquivo da SPIFFS no
  //                                  e passa o retorno para uma variável do
  //                                  tipo File.
  if (!file)  // Se houver falha ao abrir o caminho, ...
  {
    // informa ao usuário que houve falhas e sai da função retornando false.
    Serial.println(" - falha ao abrir arquivo para leitura");
    return false;
  }

  while (file.available()) // Enquanto houver algum byte disponível para
    //                          leitura do arquivo, executa o bloco de repetição.
  {
    Serial.write(file.read()); // Escreve na Serial os bytes obtidos da leitura
    //                              do arquivo.
  }
  file.close(); // Fecha o arquivo
  return true; // retorna true se não houver nenhum erro
}

/**
  Escreve o conteúdo informado em um novo arquivo da SPIFFS

  @param path : o caminho/nome do novo arquivo à ser escrito
  @param message : o conteúdo do arquivo que será gravado
  @return true se ocorreu tudo certo ou false se houve algum erro
*/
bool writeFile(String path, String message) {
  Serial.print("Gravando o arquivo ");
  Serial.print(path);
  Serial.println(" : ");

  File file = SPIFFS.open(path, FILE_WRITE); // Abre o arquivo, no modo escrita,
  //                                              onde será gravado o seu conteúdo
  //                                              e passa o retorno para
  //                                              uma variável do tipo File.
  if (!file) // Se houver falha ao abrir o caminho, ...
  {
    // informa ao usuário que houve falhas e sai da função retornando false.
    Serial.println(" - falha ao abrir arquivo para gravação");
    return false;
  }
  if (file.print(message)) // Se a escrita do arquivo com seu conteúdo der certo, ...
  {
    // informa ao usuário que deu certo
    Serial.println(" - arquivo escrito");
  } else {
    // informa ao usuário que deu erros e sai da função retornando false.
    Serial.println(" - falha na gravação do arquivo");
    return false;
  }
  file.close(); // Fecha o arquivo
  return true; // retorna true se não houver nenhum erro
}

/**
  Adiciona conteúdo informado em um arquivo existente da SPIFFS

  @param path : o caminho/nome do arquivo à ser escrito
  @param message : o conteúdo do arquivo que será anexado
  @return true se ocorreu tudo certo ou false se houve algum erro
*/
bool appendFile(String path, String message) {
  Serial.print("Anexando conteúdo ao arquivo ");
  Serial.print(path);
  Serial.println(" : ");

  File file = SPIFFS.open(path, FILE_APPEND); // Abre o arquivo, no modo anexar,
  //                                              onde será adicionado conteúdo
  //                                              e passa o retorno para
  //                                              uma variável do tipo File.

  if (!file)  // Se houver falha ao abrir o caminho, ...
  {
    // informa ao usuário que houve falhas e sai da função retornando false.
    Serial.println(" - falha ao abrir o arquivo para anexar");
    return false;
  }

  if (file.print(message)) // Se a escrita do arquivo com seu conteúdo der certo, ...
  {
    // informa ao usuário que deu certo
    Serial.println(" - mensagem anexada");
  } else {
    // informa ao usuário que deu erros e sai da função retornando false.
    Serial.println(" - Falha ao anexar");
    return false;
  }

  file.close(); // Fecha o arquivo
  return true; // retorna true se não houver nenhum erro
}

/**
  Renomeia um determinado arquivo existente da SPIFFS

  @param path1 : o caminho/nome do arquivo à ser renomeado
  @param path2 : o novo caminho/nome do arquivo
  @return true se ocorreu tudo certo ou false se houve algum erro
*/
bool renameFile(String path1, String path2) {
  Serial.print("Renomenando arquivo ");
  Serial.print(path1);
  Serial.print(" para ");
  Serial.print(path2);
  Serial.println(" : ");

  if (SPIFFS.rename(path1, path2)) // renomeia o arquivo passando o
    //                                  antigo caminho/nome (path1) e o
    //                                  novo caminho/nome (path2)
    //                                Se a renomeação der certo, ...
  {
    // informa ao usuário que deu certo
    Serial.println(" - arquivo renomeado");
  } else {
    // informa ao usuário que deu erros e sai da função retornando false.
    Serial.println(" - falha ao renomear");
    return false;
  }
  return true; // retorna true se não houver nenhum erro
}

/**
  Exclui um determinado arquivo existente da SPIFFS

  @param path : o caminho/nome do arquivo à ser excluído
  @return true se ocorreu tudo certo ou false se houve algum erro
*/
bool deleteFile(String path) {
  Serial.print("Deletando arquivo ");
  Serial.print(path);
  Serial.println(" : ");

  if (SPIFFS.remove(path)) // exclui o arquivo passando o
    //                          caminho/nome (path)
    //                        Se a exclusão der certo, ...
  {
    // informa ao usuário que deu certo
    Serial.println(" - arquivo excluído");
  } else {
    // informa ao usuário que deu erros e sai da função retornando false.
    Serial.println(" - falha na exclusão");
    return false;
  }
  return true; // retorna true se não houver nenhum erro
}

/**
  Move um determinado arquivo existente da SPIFFS para outro local

  @param pathOrigem : o caminho/nome do arquivo de origem
  @param pathDestino : o caminho/nome do arquivo de destino
  @return true se ocorreu tudo certo ou false se houve algum erro
*/
bool moveFile(String pathOrigem, String pathDestino) {
  Serial.print("Movendo arquivo ");
  Serial.print(pathOrigem);
  Serial.print(" para ");
  Serial.print(pathDestino);
  Serial.println(" : ");

  File fileOrigem = SPIFFS.open(pathOrigem); // Abre o arquivo do local de origem
  //                                              e passa o retorno para
  //                                              uma variável do tipo File.

  if (!fileOrigem)  // Se houver falha ao abrir o caminho, ...
  {
    // informa ao usuário que houve falhas e sai da função retornando false.
    Serial.println(" - falha ao abrir arquivo para cópia");
    return false;
  }

  File fileDestino = SPIFFS.open(pathDestino, FILE_WRITE); // Abre o arquivo do
  //                                                            local de destino,
  //                                                            no modo escrita,
  //                                                            e passa o retorno para
  //                                                            uma variável do tipo
  //                                                            File.

  if (!fileDestino)   // Se houver falha ao abrir o caminho, ...
  {
    // informa ao usuário que houve falhas e sai da função retornando false.
    Serial.println(" - falha ao abrir arquivo para gravação");
    return false;
  }

  while (fileOrigem.available())  // Enquanto houver algum byte disponível para
    //                                leitura do arquivo, executa o bloco de
    //                                repetição.
  {
    if (!fileDestino.write(fileOrigem.read())) // Se a escrita byte-a-byte do arquivo
      //                                            lido, também byte-a-byte, resultar
      //                                            em algum erro, ...
    {
      // informa ao usuário que deu erros e sai da função retornando false.
      Serial.println(" - falha para mover o arquivo");
      return false;
    }
  }

  //Fecha os arquivos de origem e de destino
  fileOrigem.close();
  fileDestino.close();

  if (!SPIFFS.remove(pathOrigem))  // exclui o arquivo de origem passando o
    //                                  caminho/nome (path)
    //                                Se a exclusão resultar em algum erro, ...
  {
    // informa ao usuário que deu erros e sai da função retornando false.
    Serial.println(" - falha na exclusão");
    return false;
  }
  return true; // retorna true se não houver nenhum erro
}

/**
  Copia um determinado arquivo existente da SPIFFS para outro local

  @param pathOrigem : o caminho/nome do arquivo de origem
  @param pathDestino : o caminho/nome do arquivo de destino
  @return true se ocorreu tudo certo ou false se houve algum erro
*/
bool copyFile(String pathOrigem, String pathDestino) {
  Serial.print("Copiando arquivo ");
  Serial.print(pathOrigem);
  Serial.print(" para ");
  Serial.print(pathDestino);
  Serial.println(" : ");

  File fileOrigem = SPIFFS.open(pathOrigem); // Abre o arquivo do local de origem
  //                                              e passa o retorno para
  //                                              uma variável do tipo File.

  if (!fileOrigem)   // Se houver falha ao abrir o caminho, ...
  {
    // informa ao usuário que houve falhas e sai da função retornando false.
    Serial.println(" - falha ao abrir arquivo para cópia");
    return false;
  }

  File fileDestino = SPIFFS.open(pathDestino, FILE_WRITE); // Abre o arquivo do
  //                                                            local de destino,
  //                                                            no modo escrita,
  //                                                            e passa o retorno para
  //                                                            uma variável do tipo
  //                                                            File.

  if (!fileDestino)   // Se houver falha ao abrir o caminho, ...
  {
    // informa ao usuário que houve falhas e sai da função retornando false.
    Serial.println(" - falha ao abrir arquivo para gravação");
    return false;
  }

  while (fileOrigem.available())  // Enquanto houver algum byte disponível para
    //                                leitura do arquivo, executa o bloco de
    //                                repetição.
  {
    if (!fileDestino.write(fileOrigem.read())) // Se a escrita byte-a-byte do arquivo
      //                                            lido, também byte-a-byte, resultar
      //                                            em algum erro, ...
    {
      // informa ao usuário que deu erros e sai da função retornando false.
      Serial.println(" - falha para mover o arquivo");
      return false;
    }
  }

  //Fecha os arquivos de origem e de destino
  fileOrigem.close();
  fileDestino.close();
  return true; // retorna true se não houver nenhum erro
}

/**
  Formata a SPIFFS. É excluído todos os arquivos permanentemente.

  @return true se ocorreu tudo certo ou false se houve algum erro
*/
bool formatSPIFFS() {
  //Tela de animação
  Serial.print("Formatando a partição SPIFFS. Aguarde");
  Serial.print(".");
  delay(200);
  Serial.print(".");
  delay(200);
  Serial.print(".");
  delay(200);
  Serial.print(".");
  delay(200);
  Serial.print(".");
  delay(200);
  Serial.println(".");
  delay(200);

  if (SPIFFS.format()) //Formata o sistema de arquivos SPIFFS.
    //                   Se não houver erros, ...
  {
    // informa ao usuário que deu certo.
    Serial.println(" - Formatação da partição SPIFFS concluída com sucesso!");
  } else {
    // informa ao usuário que deu erros e sai da função retornando false.
    Serial.println(" - Formatação da partição SPIFFS não foi bem - sucedida");
    return false;
  }

  return true;  // retorna true se não houver nenhum erro
}