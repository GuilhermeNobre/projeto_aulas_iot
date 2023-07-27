/*
  2.WiFi_AP.ino cria um ponto de acesso WiFi e fornece um servidor web nele.

  Passos:
  1. Conecte-se ao ponto de acesso "yourAp"
  2. Aponte seu navegador da web para http://192.168.4.1/H para ligar o LED ou http://192.168.4.1/L
  para desligá-lo OU Execute o TCP bruto "GET / H" e "GET / L" no terminal
  PuTTY com 192.168.4.1 como endereço IP e 80 como porta

  Criado para arduino-esp32 em 04 de julho de 2018
  by Elochukwu Ifediora (fedy0)
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define LED_BUILTIN 2 // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

// Defina-os como suas credenciais desejadas.
const char *ssid = "yourAP"; // TROQUE AQUI 
// Uma senha válida tem que ter mais 7 digitos
const char *password = "yourPassword"; // TROQUE AQUI

WiFiServer server(80);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configurunado access point...");

  // Você pode remover a senha se quiser para deixar acesso livre
  // if (!WiFi.softAP(ssid, password)) {

  if (!WiFi.softAP(ssid, password))
  {
    log_e("Soft AP failed.");
    while (1)
      ;
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Endereco IP: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server startk");
}

void loop()
{
  WiFiClient client = server.available(); // Procurar por clientes

  if (client)
  { // Cliente conectou
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected())
    { // Enquanto cliente estiver conectado
      if (client.available())
      {                         // Se tiver dados vindo do cliente,
        char c = client.read(); // Vai fazer leitura dos bytes
        Serial.write(c);
        if (c == '\n')
        {

          // Se a linha atual estiver em branco, você recebeu dois caracteres de nova linha seguidos.
          // Esse é o fim da solicitação HTTP do cliente, então envie uma resposta:
          if (currentLine.length() == 0)
          {

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // Aqui fica as funções de ligação que vão trabalhar na requesição
            client.print("Click <a href=\"/H\">here</a> LIGOU \"ON\" LED.<br>");
            client.print("Click <a href=\"/L\">here</a> DESLIGOU \"OFF\" LED.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else
          { // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }

        if (currentLine.endsWith("GET /H"))
        {
          digitalWrite(LED_BUILTIN, HIGH);
        }
        if (currentLine.endsWith("GET /L"))
        {
          digitalWrite(LED_BUILTIN, LOW);
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Desconectou.");
  }
}