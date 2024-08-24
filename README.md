# STM32-based-Battery-Power-Management-and-Status-Display-UPS
This is a STM32-f103c6t6 based battery power management project. It has been compilled using Arduino IDE. the key features are -
1. Measure the battery voltage and illustrate the voltage on the SSD1306 oled screen (0.0v ~ 25.0v)
2. Display the battery status (Battery low, Fully charged, Charging On, Output ON) and battery percentage (25%, 50%, 75%, 90%, 100%)
3. One relay is connected to control the charging and another relay is connected to control the output. it also has a hold function to prevent continuous connect-disconnect loop of both input and output.
4. It senses the AC input directly and control the battery load output accordingly.
    ![Screenshot (686)](https://github.com/user-attachments/assets/c335cf55-bbb4-4dc2-a9ce-35994b4ef459)
![Weixin Image_20240824185932](https://github.com/user-attachments/assets/ca3654d9-1c7c-4fb1-a426-a9dae73f9b45)
![Weixin Image_20240824185941](https://github.com/user-attachments/assets/ae618ee0-d44a-45d4-a651-975e89c4a781)
