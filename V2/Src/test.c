#include <stdio.h>
#include <unistd.h>

// Function to generate dynamic array values
void generateDynamicArray(float array[], int length) {
    for (int i = 0; i < length; ++i) {
        // Simulate dynamic data (replace this with your actual data generation logic)
        array[i] = (float)i * 1.5;
    }
}

// Function to send dynamic array over MQTT
void sendDynamicArrayOverMQTT() {
    
    // Construct the MQTT message with the dynamic array values
    printf("AT+TCMQTTPUB=\"$thing/up/property/XTRLYBWVYB/L610\",1,\"{\\\"method\\\":\\\"report\\\",\\\"clientToken\\\":\\\"123\\\",\\\"params\\\":{\\\"c1\\\":[");

    for (int i = 0; i <ADC_LENGTH ; i++) {
        printf("%d", dynamicArray[i]);
        if (i < sizeof(dynamicArray) / sizeof(dynamicArray[0]) - 1) {
            printf(", ");
        }
    }

    printf("]}}\"\r\n");
}

int main() {
    while (1) {
        sendDynamicArrayOverMQTT();
        sleep(3);  // Simulate a 3-second delay between updates
    }

    return 0;
}

//在这个示例中，generateDynamicArray函数用于生成动态数组的值，你需要将这个函数替换为你实际的数据生成逻辑。sendDynamicArrayOverMQTT函数负责构造MQTT消息，其中包含动态生成的数组值，并通过MQTT发布。

//请确保在实际应用中，你的数据生成逻辑能够根据实际需求更新数组的值。


	printf("AT+TCMQTTPUB=\"$thing/up/property/RMRPC21BU5/board1\",1,\"{\\\"method\\\":\\\"report\\\",\\\"clientToken\\\":\\\"123\\\",\\\"params\\\":{\\\"heartbeat\\\":%d}}\"\r\n",heartbeat);
		//printf("AT+TCMQTTPUB=\"$thing/up/property/RMRPC21BU5/board1",117,"{"method":"report","clientToken":"v2530389123ZckrJ::14ab9571-92a0-4156-ada2-de96628ea4c2","params":{"heartbeat":70}}"\r\n");
        HAL_Delay(1000);
		strx=strstr((const char*)RxBuffer,(const char*)"success");
		while(strx==NULL)
		{
			Clear_Buffer();	
			printf("上报失败");
			HAL_Delay(1000);
		printf("AT+TCMQTTPUB=\"$thing/up/property/RMRPC21BU5/board1\",1,\"{\\\"method\\\":\\\"report\\\",\\\"clientToken\\\":\\\"123\\\",\\\"params\\\":{\\\"heartbeat\\\":%d}}\"\r\n",heartbeat);
		
           
            
            
            //	printf("AT+TCMQTTPUB=\"$thing/up/property/RMRPC21BU5/board1",117,"{"method":"report","clientToken":"v2530389123ZckrJ::14ab9571-92a0-4156-ada2-de96628ea4c2","params":{"heartbeat":70}}"\r\n");
            HAL_Delay(1000);
			strx=strstr((const char*)RxBuffer,(const char*)"success");
		}
		Clear_Buffer();
		printf("上报成功");
		HAL_Delay(3000);