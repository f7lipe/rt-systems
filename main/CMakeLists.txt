idf_component_register(SRCS 
                        "main.c"
                        "src/ui/ui.c"  # Inclua o arquivo ui.c
                        "src/hardware/led/led.c"  # Inclua o arquivo led.c
                        "src/hardware/display/display.c"  # Inclua o arquivo display.c
                        "src/hardware/microphone/mic.c" # Inclua o arquivo mic.c
                        "src/dsp/signal.c" # Inclua o arquivo signal.c
                    INCLUDE_DIRS 
                    "src/ui" 
                    "src/hardware/led" 
                    "src/hardware/display" 
                    "src/hardware/microphone"
                    "src/dsp"
                    ".")
                    

# Define o nome do executável
set(TARGET_NAME "cyclical")

# Define os arquivos fonte do seu projeto (incluindo o main.c)
set(SOURCES
    main.c
    src/hardware/led/led.c
    src/hardware/display/display.c
    src/hardware/microphone/mic.c
    src/dsp/signal.c
    src/ui/ui.c
    # Outros arquivos fonte do seu projeto, se houver
)

# Define a pasta de inclusão para os cabeçalhos do PortAudio (opcional, se necessário)
include_directories(/usr/local/include/)

# Encontra a biblioteca PortAudio
find_library(PORTAUDIO_LIBRARY
             NAMES portaudio
             PATHS /usr/local/lib)

# Adiciona o executável do seu projeto
add_executable(${TARGET_NAME} ${SOURCES})

# Vincula a biblioteca PortAudio ao executável
target_link_libraries(${TARGET_NAME} ${PORTAUDIO_LIBRARY})