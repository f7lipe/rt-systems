idf_component_register(SRCS 
                        "main.c"
                        "src/ui/ui.c"  # Inclua o arquivo ui.c
                        "src/hardware/led/led.c"  # Inclua o arquivo led.c
                        "src/hardware/display/display.c"  # Inclua o arquivo display.c
                        "src/hardware/microphone/mic.c" # Inclua o arquivo mic.c
                        #"src/hardware/bluetooth/bluetooth.c" # Inclua o arquivo bluetooth.c
                        "src/hardware/button/buttons.c" # Inclua o arquivo buttons.c
                        "src/dsp/signal.c" # Inclua o arquivo signal.c
                    
                    INCLUDE_DIRS 
                    "src/ui" 
                    "src/hardware/led" 
                    "src/hardware/display" 
                    "src/hardware/microphone"
                    #"src/hardware/bluetooth"
                    "src/hardware/button"
                    "src/dsp"
                    ".")