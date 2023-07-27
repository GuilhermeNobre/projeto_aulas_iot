const char index[] PROGMEM = R"rawliteral(
    <!DOCTYPE html>
<html lang='en'>
<head>
    <meta charset='UTF-8'>
    <meta http-equiv='X-UA-Compatible' content='IE=edge'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Módulo Rele</title>
    <link rel='stylesheet' href='./style.css'>
</head>
<body>
    <script>

        function OnClick()
        {
            var btOn = document.getElementById('btOn').disabled = true;
            var btOff = document.getElementById('btOff');
            if(btOff.disabled == true) btOff.disabled = false;
            
            const xhr = new XMLHttpRequest();
            xhr.open('GET', 'low');
            xhr.onload = function()
            {
                if(xhr.status === 200)
                {
                    if(btOff.disabled == true) btOff.disabled = false;
                }
                else
                {
                    console.log('Faield');
                }
            };
            xhr.send();
        }

        function OffClick()
        {
            var btOff = document.getElementById('btOff').disabled = true;
            var btOn = document.getElementById('btOn');
            if(btOn.disabled == true) btOn.disabled = false;
            
            const xhr = new XMLHttpRequest();
            xhr.open('GET', 'high');
            xhr.onload = function()
            {
                if(xhr.status === 200)
                {
                    if(btOn.disabled == true) btOn.disabled = false;
                }
                else
                {
                    console.log('Faield');
                }
            };
            xhr.send();
        }

    </script>
    <div class='center-screen'>
        <div class='center-div'>
            <h2>Pressione os botões</h2>
                <button id='btOn' class='buttonOn' onclick='OnClick()'>ON </button>
                <button id='btOff' class='buttonOff' onclick='OffClick()'>OFF</button>
        </div>
    </div>
</body>
</html>)rawliteral";