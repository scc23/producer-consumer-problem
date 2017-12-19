//////////////////////////////////////////////////////////////////////////////
// Producer-Consumer Problem (Bounded Buffer Problem)
//
// Author: Sherman Chow
// Date: October 23, 2017
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <termios.h>

// Define the maximum buffer size
#define MAX_BUFFER_SIZE 10

// Struct to store operator materials and tools
struct operator_struct {
	int operator_materials[2];
	int operator_tools[2];
};

// Initialize mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Initialize buffer counter
int buffer_counter = 0;
int material1_counter = 0;
int material2_counter = 0;
int material3_counter = 0;
int material1_in_buffer = 0;
int material2_in_buffer = 0;
int material3_in_buffer = 0;

// Initialize tools counter
int tools_counter = 0;

// Initialize output queue counter
int output_queue_counter = 0;

// Initialize products counter
int productA_counter = 0;
int productB_counter = 0;
int productC_counter = 0;

// Variable to wait for initial buffer to fill before forwarding materials to operators
int initial_grab = 1;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Linked list for input buffer, tools, and output queue
struct Node
{
    int data;
    struct Node* next;
};

// Count number of elements in linked list
int countElements(struct Node **head)
{
    struct Node * current = *head;
    int length = 0;
    
    while (current != NULL)
    {
        length++;
        current = current->next;
    }
    return length;
}

// Print elements of linked list
void printList(struct Node **head)
{
    struct Node * current = *head;

    while (current != NULL)
    {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

// Print elements of output queue
void printOutputQueue(struct Node **head)
{
	struct Node * current = *head;

	while (current != NULL)
	{
		if (current->data == 111)
			printf("A ");
		else if (current->data == 222)
			printf("B ");
		else if (current->data == 333)
			printf("C ");

		current = current->next;
	}
	printf("\n");
}

// Append linked list
void appendNode(struct Node **head, int data)
{
    struct Node * current = *head;
    struct Node * newNode = malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;

    if (current == NULL)
    {
        *head = newNode;
    }
    else
    {
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Remove Nth node in linked list
int removeNode(struct Node **head, int index)
{
    struct Node * temp = *head;
    int ret;

    // If index is at head
    if (index == 0)
    {
        ret = temp->data;
        *head = temp->next;
        free(temp);
        return ret;
    }

    // Find previous node of node to be removed
    for (int i = 0; temp != NULL && i < index-1; i++)
    {
        temp = temp->next;
    }

    // Node temp->next is node to be removed
    // Store pointer to next of node to be deleted
    struct Node * next = temp->next->next;
    ret = temp->next->data;
    // Unlink node from linked list
    free(temp->next);
    // Unlink deleted node from linked list
    temp->next = next;
    return ret;
}

// Get the Nth node data
int getNodeData(struct Node **head, int index)
{
	struct Node * current = *head;
	int count = 0;
	int ret = -1;

	// If index is at head
	if (index == 0)
	{
		ret = current->data;
		return ret;
	}

	while (current != NULL)
	{
		if (count == index)
		{
			ret = current->data;
			return ret;
		}
		count++;
		current = current->next;
	}

	// Not found
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialize materials
struct Node * materials = NULL;
// Initialize tools
struct Node * tools = NULL;
// Initialize output queue
struct Node * output_queue = NULL;

// Insert material to buffer
int insert_material(int material)
{
	// If buffer is not full, insert material
	if (buffer_counter < MAX_BUFFER_SIZE)
	{
		// Keep track of number of materials within buffer
		if (material == 1)
			material1_in_buffer++;
		else if (material == 2)
			material2_in_buffer++;
		else if (material == 3)
			material3_in_buffer++;

		appendNode(&materials, material);
		buffer_counter++;
		printf("- Parameter 2 has changed -\n");
		printf("Material %d has been added into input buffer\n", material);
		printf("Input Buffer: ");
		printList(&materials);
		printf("Input Buffer Size: %d\n", buffer_counter);
		printf("\n");
		return 0;
	}
	// If buffer is full
	else
	{
		return -1;
	}
}

/// Remove material from buffer
int remove_material(int position)
{
	int material = getNodeData(&materials, position);
	// If buffer is not empty, remove material
	if (buffer_counter > 0)
	{
		// Keep track of number of materials within buffer
		if (material == 1)
			material1_in_buffer--;
		else if (material == 2)
			material2_in_buffer--;
		else if (material == 3)
			material3_in_buffer--;

		removeNode(&materials, position);
		buffer_counter--;
		printf("- Parameter 2 has changed -\n");
		printf("Material %d has been removed from input buffer\n", material);
		printf("Input Buffer: ");
		printList(&materials);
		printf("Input Buffer Size: %d\n", buffer_counter);
		printf("\n");
		return 0;
	}
	// If buffer is empty
	else
	{
		return -1;
	}
}

// // Insert tool
void insert_tool(int tool)
{
	appendNode(&tools, tool);
	tools_counter++;
}

// Remove tool
int remove_tool(int index)
{
	// If tools not empty, remove tool
	if (tools_counter > 0)
	{
		removeNode(&tools, index);
		tools_counter--;
		return 0;
	}
	// If tools empty, error
	else
	{
		return -1;
	}
}

// Insert product to output queue
void insert_product(int product)
{
	char pro = ' ';
	appendNode(&output_queue, product);
	output_queue_counter++;
	if (product == 111)
	{
		productA_counter++;
		pro = 'A';
	}
	else if (product == 222)
	{
		productB_counter++;
		pro = 'B';
	}
	else if (product == 333)
	{
		productC_counter++;
		pro = 'C';
	}
	printf("- Parameter 3 has changed -\n");
	printf("Product %c has been produced\n", pro);
	printf("Product A: %d produced\n", productA_counter);
	printf("Product B: %d produced\n", productB_counter);
	printf("Product C: %d produced\n", productC_counter);
	printf("\n");
	printf("- Parameter 4 has changed -\n");
	printf("Output Queue: ");
	printOutputQueue(&output_queue);
	printf("Output Queue Size: %d\n", output_queue_counter);
	printf("\n");
}

// Producer thread for generator
void *generator_producer(void *param)
{
	// Initialize buffer material
	int material = *(int *) param;

	// Infinite loop
	while (1)
	{
		// Start critical section
		pthread_mutex_lock(&mutex);

		if (initial_grab == 1 && buffer_counter < MAX_BUFFER_SIZE)
		{
			if (material == 1)
			{
				// Conditions to have a good variety of materials in the input buffer
				if (material1_in_buffer - material2_in_buffer < 3 && material1_in_buffer - material3_in_buffer < 3
					&& material1_counter - material2_counter < 3 && material1_counter - material3_counter < 3)
				{
					if (insert_material(material) == 0)
					{
						material1_counter++;
						printf("- Parameter 1 has changed -\n");
						printf("Material 1 has been generated\n");
						printf("Material 1: %d generated\n", material1_counter);
						printf("Material 2: %d generated\n", material2_counter);
						printf("Material 3: %d generated\n", material3_counter);
						printf("\n");
					}
				}
			}
			else if (material == 2)
			{
				// Conditions to have a good variety of materials in the input buffer
				if (material2_in_buffer - material1_in_buffer < 3 && material2_in_buffer - material3_in_buffer < 3
					&& material2_counter - material1_counter < 3 && material2_counter - material3_counter < 3)
				{
					if (insert_material(material) == 0)
					{
						material2_counter++;
						printf("- Parameter 1 has changed -\n");
						printf("Material 2 has been generated\n");
						printf("Material 1: %d generated\n", material1_counter);
						printf("Material 2: %d generated\n", material2_counter);
						printf("Material 3: %d generated\n", material3_counter);
						printf("\n");
					}
				}
			}
			else if (material == 3)
			{
				// Conditions to have a good variety of materials in the input buffer
				if (material3_in_buffer - material1_in_buffer < 3 && material3_in_buffer - material2_counter < 3
					&& material3_counter - material1_counter < 3 && material3_counter - material2_counter < 3)
				{
					if (insert_material(material))
					{
						material3_counter++;
						printf("- Parameter 1 has changed -\n");
						printf("Material 3 has been generated\n");
						printf("Material 1: %d generated\n", material1_counter);
						printf("Material 2: %d generated\n", material2_counter);
						printf("Material 3: %d generated\n", material3_counter);
						printf("\n");
					}
				}
			}
		}
		// After the initial operator has grabbed a material, leave space in buffer for operators to put back materials
		else if (initial_grab == 0 && buffer_counter < MAX_BUFFER_SIZE-2)
		{
			if (material == 1)
			{
				// Conditions to have a good variety of materials in the input buffer
				if (buffer_counter == 0 
					|| (material1_in_buffer < 3 && material2_in_buffer > 0)
					|| (material1_in_buffer < 3 && material3_in_buffer > 0))
				{
					if (insert_material(material) == 0)
					{
						material1_counter++;
						printf("- Parameter 1 has changed -\n");
						printf("Material 1 has been generated\n");
						printf("Material 1: %d generated\n", material1_counter);
						printf("Material 2: %d generated\n", material2_counter);
						printf("Material 3: %d generated\n", material3_counter);
						printf("\n");
					}
				}
			}
			else if (material == 2)
			{
				// Conditions to have a good variety of materials in the input buffer
				if (buffer_counter == 0 
					|| (material2_in_buffer < 3 && material1_in_buffer > 0)
					|| (material2_in_buffer < 3 && material3_in_buffer > 0))
				{
					if (insert_material(material) == 0)
					{
						material2_counter++;
						printf("- Parameter 1 has changed -\n");
						printf("Material 2 has been generated\n");
						printf("Material 1: %d generated\n", material1_counter);
						printf("Material 2: %d generated\n", material2_counter);
						printf("Material 3: %d generated\n", material3_counter);
						printf("\n");
					}
				}
			}
			else if (material == 3)
			{
				// Conditions to have a good variety of materials in the input buffer
				if (buffer_counter == 0 
					|| (material3_in_buffer < 3 && material1_in_buffer > 0)
					|| (material3_in_buffer < 3 && material2_in_buffer > 0))
				{
					if (insert_material(material) == 0)
					{
						material3_counter++;
						printf("- Parameter 1 has changed -\n");
						printf("Material 3 has been generated\n");
						printf("Material 1: %d generated\n", material1_counter);
						printf("Material 2: %d generated\n", material2_counter);
						printf("Material 3: %d generated\n", material3_counter);
						printf("\n");
					}
				}
			}
		}
		// End critical section
		pthread_mutex_unlock(&mutex);
	}
}

// Consumer thread for operator materials
void *operator_consumer_materials(void *param)
{
	// Initialize variables
	struct operator_struct *operator = (struct operator_struct *) param;	
	srand(time(NULL));
	int material_position, material;

	// Infinite loop
	while (1)
	{
		// Start critical section
		pthread_mutex_lock(&mutex);

		// Grab materials
		if (initial_grab == 1)
		{
			if (buffer_counter == MAX_BUFFER_SIZE)
			{
				// Randomly select position to grab material from buffer
				material_position = rand() % buffer_counter;
				material = getNodeData(&materials, material_position);
				// Select again until material is different from first material
				while (material == operator->operator_materials[0])
				{
					material_position = rand() % buffer_counter;
					material = getNodeData(&materials, material_position);
				}

				remove_material(material_position);
				operator->operator_materials[0] = material;

				initial_grab = 0;
			}
		}
		else if (buffer_counter > 0 && (operator->operator_materials[0] == 0 || operator->operator_materials[1] == 0))
		{
			// Attempt to remove material
			if (buffer_counter > 0)
			{
				// Randomly select position to grab material from buffer
				material_position = rand() % buffer_counter;
				material = getNodeData(&materials, material_position);
				// Select again until material is different from first material
				while (material == operator->operator_materials[0])
				{
					material_position = rand() % buffer_counter;
					material = getNodeData(&materials, material_position);
				}

				// Remove material from buffer
                remove_material(material_position);
                // Insert material to operator storage
                if (operator->operator_materials[0] == 0)
                    operator->operator_materials[0] = material;
                else
                    operator->operator_materials[1] = material;
			}
		}
		// End critical section
		pthread_mutex_unlock(&mutex);
	}
}

// Consumer thread for operator tools
void *operator_consumer_tools(void *param)
{
	// Initialize variables
	struct operator_struct *operator = (struct operator_struct *) param;	
	srand(time(NULL));
	int tool_position1, tool_position2, tool1, tool2;

	// Infinite loop
	while (1)
	{
		// Start critical section
		pthread_mutex_lock(&mutex);

		// Grab pair of tools
		if (tools_counter > 1 && operator->operator_tools[0] == 0 && operator->operator_tools[1] == 0)
		{
			// Grab tool1
			tool_position1 = rand() % tools_counter;
			tool1 = getNodeData(&tools, tool_position1);
			remove_tool(tool_position1);
			operator->operator_tools[0] = tool1;

			// Grab tool2
			tool_position2 = rand() % tools_counter;
			tool2 = getNodeData(&tools, tool_position2);
			// Select again until tool is different from tool1
			while (tool2 == tool1)
			{
				tool_position2 = rand() % tools_counter;
				tool2 = getNodeData(&tools, tool_position2);
			}
			remove_tool(tool_position2);
			operator->operator_tools[1] = tool2;

		}

		// End critical section
		pthread_mutex_unlock(&mutex);
	}
}

// Producer thread for operator
void *operator_producer(void *param)
{
	// Initialize variables
	struct operator_struct *operator = (struct operator_struct *) param;
	srand(time(NULL));
	int product;
	float random_time;

	// Infinite loop
	while (1)
	{
		// Operator takes between 0.01s to 1s to produce a product
		random_time = ((float)rand()) / RAND_MAX;
		sleep(random_time);

		product = 0;
		
		// Start critical section
		pthread_mutex_lock(&mutex);

		// If all materials and tools are obtained, produce product
		if (operator->operator_materials[0] != 0 && operator->operator_materials[1] != 0 && operator->operator_tools[0] != 0 && operator->operator_tools[1] != 0)
		{
			// Check if product A
			if ( ((operator->operator_materials[0] == 1 && operator->operator_materials[1] == 2) || (operator->operator_materials[0] == 2 && operator->operator_materials[1] == 1)) 
				&& ((operator->operator_tools[0] == 11 && operator->operator_tools[1] == 22) || (operator->operator_tools[0] == 22 && operator->operator_tools[1] == 11)) )
			{
				// Difference of numnber of 2 products must be less than 10
				if (productA_counter - productB_counter < 9 && productA_counter - productC_counter < 9)
					product = 111;
				else
				{
					// Put tools back
                    insert_tool(operator->operator_tools[0]);
                    insert_tool(operator->operator_tools[1]);
                    operator->operator_tools[0] = 0;
                    operator->operator_tools[1] = 0;
                    // Put materials back
                    if (insert_material(operator->operator_materials[0]) == 0)
					operator->operator_materials[0] = 0;
                    if (insert_material(operator->operator_materials[1]) == 0)
					operator->operator_materials[1] = 0;
				}
			}
			// Check if product B
			else if ( ((operator->operator_materials[0] == 2 && operator->operator_materials[1] == 3) || (operator->operator_materials[0] == 3 && operator->operator_materials[1] == 2)) 
				&& ((operator->operator_tools[0] == 22 && operator->operator_tools[1] == 33) || (operator->operator_tools[0] == 33 && operator->operator_tools[1] == 22)) )
			{
				// Difference of numnber of 2 products must be less than 10
				if (productB_counter - productA_counter < 9 && productB_counter - productC_counter < 9)
					product = 222;
				else
				{
					// Put tools back
                    insert_tool(operator->operator_tools[0]);
                    insert_tool(operator->operator_tools[1]);
                    operator->operator_tools[0] = 0;
                    operator->operator_tools[1] = 0;
                    // Put materials back
                    if (insert_material(operator->operator_materials[0]) == 0)
						operator->operator_materials[0] = 0;
                    if (insert_material(operator->operator_materials[1]) == 0)
						operator->operator_materials[1] = 0;
				}
			}
			// Check if product C
			else if ( ((operator->operator_materials[0] == 3 && operator->operator_materials[1] == 1) || (operator->operator_materials[0] == 1 && operator->operator_materials[1] == 3)) 
				&& ((operator->operator_tools[0] == 33 && operator->operator_tools[1] == 11) || (operator->operator_tools[0] == 11 && operator->operator_tools[1] == 33)) )
			{
				// Difference of numnber of 2 products must be less than 10
				if (productC_counter - productA_counter < 9 && productC_counter - productB_counter < 9)
					product = 333;
				else
				{
					// Put tools back
                    insert_tool(operator->operator_tools[0]);
                    insert_tool(operator->operator_tools[1]);
                    operator->operator_tools[0] = 0;
                    operator->operator_tools[1] = 0;
                    // Put materials back
                    if (insert_material(operator->operator_materials[0]) == 0)
						operator->operator_materials[0] = 0;
                    if (insert_material(operator->operator_materials[1]) == 0)
						operator->operator_materials[1] = 0;
				}
			}
			// If they do not make products A, B, or C, then put back materials and tools
			else
			{
				// Put tools back
				insert_tool(operator->operator_tools[0]);
				insert_tool(operator->operator_tools[1]);
				operator->operator_tools[0] = 0;
				operator->operator_tools[1] = 0;
				// Put materials back
				if (insert_material(operator->operator_materials[0]) == 0)
					operator->operator_materials[0] = 0;
				if (insert_material(operator->operator_materials[1]) == 0)
					operator->operator_materials[1] = 0;
			}

			// Insert product
			if (product == 111 || product == 222 || product == 333)
			{
				// Check if two same products will be next to each other
				if (output_queue_counter > 0 && getNodeData(&output_queue, output_queue_counter-1) == product)
				{
					// Put tools back
                    insert_tool(operator->operator_tools[0]);
                    insert_tool(operator->operator_tools[1]);
                    operator->operator_tools[0] = 0;
                    operator->operator_tools[1] = 0;
                    // Put materials back
                    if (insert_material(operator->operator_materials[0]) == 0)
						operator->operator_materials[0] = 0;
                    if (insert_material(operator->operator_materials[1]) == 0)
						operator->operator_materials[1] = 0;
				}
				else
				{
					insert_product(product);
					// Empty out materials
					operator->operator_materials[0] = 0;
					operator->operator_materials[1] = 0;
					// Put tools back
					insert_tool(operator->operator_tools[0]);
					insert_tool(operator->operator_tools[1]);
					operator->operator_tools[0] = 0;
					operator->operator_tools[1] = 0;
				}
			}
		}
		// End critical section
		pthread_mutex_unlock(&mutex);
	}
}

// Function for getch, used for pausing and resuming program
int getch(void)
{
        int c;
        struct termios old_attr, new_attr;

		tcgetattr(STDIN_FILENO, &old_attr);
		
        new_attr = old_attr;
		new_attr.c_lflag &= ~(ICANON|ECHO);
		
		tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);
		
		c = getchar();
		
        tcsetattr(STDIN_FILENO, TCSANOW, &old_attr);
        
        return c;
}

// Thread function to pause and resume program
void *pause_resume(void *param)
{
	int isPaused = 0;
	char key;

	// Infinite loop
	while (1)
	{
		key = getch();
		// Pause program if user pressed 'p'
		if (key == 'p')
		{
			if (isPaused == 0)
			{
				pthread_mutex_lock(&mutex);
				printf("\n\n--- PROGRAM PAUSED ---\n");
				printf("\nStatus of current parameters:\n\n");
				printf("Parameter 1.\nMaterial 1: %d generated\n", material1_counter);
				printf("Material 2: %d generated\n", material2_counter);
				printf("Material 3: %d generated\n\n", material3_counter);
				printf("Parameter 2.\nInput Buffer: ");
				printList(&materials);
				printf("Input Buffer Size: %d\n\n", buffer_counter);
				printf("Parameter 3.\nProduct A: %d produced\n", productA_counter);
				printf("Product B: %d produced\n", productB_counter);
				printf("Product C: %d produced\n\n", productC_counter);
				printf("Parameter 4.\nOutput Queue: ");
				printOutputQueue(&output_queue);
				printf("Output Queue Size: %d\n", output_queue_counter);
				printf("\n\nEnter 'r' to resume the program.\n");

				isPaused = 1;
			}
		}
		// Resume the program if user pressed 'r'
		else if (key == 'r')
		{
			isPaused = 0;
			pthread_mutex_unlock(&mutex);
		}
	}
}

// Main function
int main(int argc, char *argv[])
{
	// Initialize tools
	insert_tool(11);
	insert_tool(22);
	insert_tool(33);

	// Spawn threads
	pthread_t tid;	// Thread ID

	// Create generator producer threads
	int material1 = 1;
	int material2 = 2;
	int material3 = 3;
	pthread_create(&tid, NULL, generator_producer, &material1);
	pthread_create(&tid, NULL, generator_producer, &material2);
	pthread_create(&tid, NULL, generator_producer, &material3);

	// Initialize structs for operators
    struct operator_struct operator1;
    operator1.operator_materials[0] = 0;
    operator1.operator_materials[1] = 0;
    operator1.operator_tools[0] = 0;
    operator1.operator_tools[1] = 0;

    struct operator_struct operator2;
    operator2.operator_materials[0] = 0;
    operator2.operator_materials[1] = 0;
    operator2.operator_tools[0] = 0;
    operator2.operator_tools[1] = 0;

    struct operator_struct operator3;
    operator3.operator_materials[0] = 0;
    operator3.operator_materials[1] = 0;
    operator3.operator_tools[0] = 0;
	operator3.operator_tools[1] = 0;

    // Create operator consumer/producer threads
	pthread_create(&tid, NULL, operator_consumer_materials, (void*)&operator1);
	pthread_create(&tid, NULL, operator_consumer_tools, (void*)&operator1);
	pthread_create(&tid, NULL, operator_producer, (void*)&operator1);

	pthread_create(&tid, NULL, operator_consumer_materials, (void*)&operator2);
	pthread_create(&tid, NULL, operator_consumer_tools, (void*)&operator2);
	pthread_create(&tid, NULL, operator_producer, (void*)&operator2);

	pthread_create(&tid, NULL, operator_consumer_materials, (void*)&operator3);
	pthread_create(&tid, NULL, operator_consumer_tools, (void*)&operator3);
	pthread_create(&tid, NULL, operator_producer, (void*)&operator3);

	// Create thread for pausing/resuming program
	pthread_create(&tid, NULL, pause_resume, NULL);

	// Wait for thread termination
	pthread_join(tid, NULL);

	return 0;
}
