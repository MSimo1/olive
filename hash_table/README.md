# Compilare i file C da terminale

Questo progetto usa codice C e va compilato con GCC.

## Dipendenze richieste

- GCC / MinGW
- PowerShell o terminale Windows
- un file `.c` da compilare

## Installazione di GCC (Windows)

Se non hai ancora un compilatore, installa MinGW via WinGet:

```powershell
winget install --id BrechtSanders.WinLibs.POSIX.UCRT -e --accept-source-agreements --accept-package-agreements
```

Questo installa GCC in una cartella simile a:

```text
C:\Users\ideha\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin
```

## Rendi `gcc` visibile da terminale

Per renderlo disponibile senza scrivere il percorso completo, aggiungi la cartella al PATH utente:

```powershell
$gccDir = "C:\Users\ideha\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin"
$userPath = [Environment]::GetEnvironmentVariable("Path", "User")
if (-not ($userPath -split ';' | Where-Object { $_ -eq $gccDir })) {
    [Environment]::SetEnvironmentVariable("Path", "$userPath;$gccDir", "User")
}
```

Poi chiudi e riapri il terminale.

## Verifica che GCC funzioni

```powershell
gcc --version
```

Se appare la versione di GCC, il compilatore è pronto.

## Compilare un file C

Se il file è `bonjour.c`:

```powershell
gcc bonjour.c -o bonjour
```

Questo crea un eseguibile chiamato `bonjour.exe`.

Per eseguirlo:

```powershell
.\bonjour.exe
```

## Esempio completo

```powershell
cd C:\Users\ideha\Desktop\Maxim\Progetto\olive\hash_table
gcc bonjour.c -o bonjour
.\bonjour.exe
```

Output previsto:

```text
bonjour
```

## Se vuoi compilare un file diverso

```powershell
gcc operazione_get.c -o operazione_get
.\operazione_get.exe
```

## Nota importante

- `gcc file.c` compila e crea un file eseguibile di default (`a.exe` su Windows)
- `gcc file.c -o nome` permette di scegliere il nome del file eseguibile
- per vedere il risultato, devi eseguire il file `.exe` dopo la compilazione
