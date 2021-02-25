#include "RSA.h"


vector<double> RSA_primeNumbers(void)
{
	cout << " ...research of prime numbers :" << endl;

	double first_rangeBoundary, last_rangeBoundary = 0;

	goto recycle;

	recycle:

	cout << endl <<" --- Type the first boundary of searching range -> " << endl;

	cin >> first_rangeBoundary;

	cout << endl << " --- Type the second boundary of searching range ->" << endl;

	cin >> last_rangeBoundary;

	while (last_rangeBoundary <= first_rangeBoundary || ((last_rangeBoundary - first_rangeBoundary) < MIN_RANGE))
	{
		if (last_rangeBoundary < first_rangeBoundary && (first_rangeBoundary - last_rangeBoundary < MIN_RANGE))
		{
			cout << endl << "...first boundary is bigger than second; second must be at least 5 times bigger then first ! ...retype second boundary! " << endl;

			cin >> last_rangeBoundary;
		}
		else if (last_rangeBoundary < first_rangeBoundary)
		{
			cout << endl << "...first boundary is bigger than second; inverted range will be used..." << endl;

			double temp = last_rangeBoundary;

			last_rangeBoundary = first_rangeBoundary;

			first_rangeBoundary = temp;
		}
		else if (last_rangeBoundary == first_rangeBoundary)
		{
			cout << endl << " ...are equal ...retype a different second boundary! " << endl;

			cin >> last_rangeBoundary;
		}
		else if (last_rangeBoundary - first_rangeBoundary < MIN_RANGE)
		{
			cout << endl << "...second must be at least 5 times bigger then first ! Retype a bigger second boundary! " << endl;

			cin >> last_rangeBoundary;
		}
	}

	cout << endl << " ...research of prime numbers between " << first_rangeBoundary << " and " << last_rangeBoundary << " started ..." << endl;

	vector<double> primeNumbers; // Declare a container without initilazize it. Containers are a type of standard array, but dynamic

	for (double i = first_rangeBoundary; i <= last_rangeBoundary; i++)
	{
		for (double j = 2; j <= i; j++) 
		{
			if (j == i)
			{
				primeNumbers.push_back(i); // Append the prime numbers one by one in this container
			}

			if ((int)i % (int)j == 0) // Cast to int is lossless because the used double haven't a decimal part
			{
				break; // if there is at least one divisor such that the remainder of division is equal to zero, with dividena and divisor different, this i aren't a prime number
			}
		}
	}

	cout << endl << " ...finded # " << primeNumbers.size() << " prime numbers : ";

	if (primeNumbers.size() <= 0) // check for security, if there is only one prime number in the selected range (but with previous range width check, this is impossible...)
	{
		cout << endl << "ATTENTION! ...there is only one prime number in range!!" << endl << endl;

		goto recycle; // return in previous position

	}

	for (long int i = 0; i < primeNumbers.size(); i++)
	{
		cout << " " << primeNumbers[i];
	}

	return primeNumbers;
}


array<double, 3> RSA_selectTwoRandom(vector<double> primeNumbers)
{
	cout << endl << endl << " ...chosing random p and q between prime numbers in setted range... " << endl;

	srand(time(0));

	double rand_i = (double)((rand() % (primeNumbers.size()) + 0));
	double rand_j = (double)((rand() % (primeNumbers.size()) + 0));

	while (rand_i >= sizeof(primeNumbers) || rand_j >= sizeof(primeNumbers) || rand_i == rand_j) // additional control of picking value in defined range and picking different value
	{
		rand_i = (double)((rand() % (primeNumbers.size()) + 0)); // ...reselect a random number
		rand_j = (double)((rand() % (primeNumbers.size()) + 0));
	}

	double i = primeNumbers[rand_i]; // select number in random position beetween elements of vector of prime numbers
	double j = primeNumbers[rand_j]; // ...select another different number 

	array<double, 3> randomPrimes = {i, j, (i*j)}; // in third array position, place value of n; 

	cout << endl << " p = " << randomPrimes[0] << endl << " q = " << randomPrimes[1] << endl << " n = " << randomPrimes[2] << endl;

	return randomPrimes;
}


double RSA_compute_totient(array<double, 3> randomPrimes) 
{
	double totient = ((randomPrimes[0] - 1) * (randomPrimes[1] - 1)); // totient is the result of Euler function when n = p * q, and p, q are prime numbers 

	cout << endl << " totient = " << totient << endl;

	return totient;
}


double RSA_compute_e(double totient) // e is our private key
{
	srand(time(0));

	vector<double> totient_divisor;
	vector<double> e_divisor;

	cout << " totient divisor : ";

	for (double i = 2; i <= totient; i++)
	{
		if ((int)totient % (int)i == 0) // cast to int is lossless because the used double haven't a decimal part
		{
			totient_divisor.push_back(i);

			cout << " " << i;
		}
	}

	//long int rand_e = (rand() % totient + 0); // ...the right formula...

	double rand_e = (double)(rand() % 8 + 2); // Restric value of e to improve operations speed and AVOID OVERFLOW in next operations...
	
	while (rand_e >= totient) // max common divisor between e and totient must be 1... for first, check if e and totient are different 
	{
		//rand_e = (rand() % totient + 2); // ...the right formula...

		rand_e = (double)(rand() % 8 + 2); // Restric value of e to improve operations speed and AVOID OVERFLOW in next operations...
	}

	cout << endl << endl << " ...test coprime requirement between totient and e = " << rand_e << endl;

	cout << " e divisor : ";

	for (double i = 2; i <= rand_e; i++) // find the divisor of e
	{
		if ((int)rand_e % (int)i == 0) // Cast to int is lossless because the used double haven't a decimal part
		{
			e_divisor.push_back(i); // Append in e divisor container

			cout << " " << i;
		}
	}

	goto recycle;

	recycle:

	for (long int i = 0; i < totient_divisor.size(); ++i) // mcd between e and totient must be 1
	{
		for (long int j = 0; j < e_divisor.size(); ++j)
		{
			if (e_divisor[j] == totient_divisor[i]) // If one e divisor (different to 1..) is equal to the totient divisor, e and totient aren't coprime!. Need to recalculate e...
			{
				cout << endl << " e = " << rand_e << " --> NO " << endl;

				e_divisor.clear(); // All elements to zero

				//rand_e = (rand() % totient + 2);

				rand_e = (double)(rand() % 8 + 2); // restric value of e to improve operations speed and AVOID OVERFLOW in next operations...

				while (rand_e > totient)
				{
					//rand_e = (rand() % totient + 0);

					rand_e = (double)(rand() % 8 + 2); // restric value of e to improve operations speed and AVOID OVERFLOW in next operations...
				}

				cout << endl << endl << " ...test coprime requirement between totient and e = " << rand_e << endl;

				cout << " e divisor : ";

				for (double k = 2; k <= rand_e; k++) // look again for the divisor of new e
				{
					if ((int) rand_e % (int)k == 0) // cast to int is lossless because the used double haven't a decimal part
					{
						e_divisor.push_back(k); // Replace new divisor in container

						cout << " " << k;
					}
				}

				goto recycle; // return in previous code position to recheck if e and totient are coprime numbers
			}
		}
	}

	cout << endl << " e = " << rand_e << " --> OK " << endl;

	return rand_e;
}


double RSA_compute_d(double totient, double e) // d is our public key
{
	double d = 2; // Traial start with d = 2

	cout << endl << endl << " ...test if d = " << d << " is the inverse of e = " << e << endl;

	while (fmod(d * e, totient) != 1) // check one by one is an easy way to find d
	{
		cout << endl << " d = " << d << " --> NO " << endl;

		d++;
	}

	cout << endl << " d = " << d << " --> OK " << endl;

	return d;
}


string RSA_decryptMessage(vector<double> encriptedMessage, double d, double n)
{
	cout << endl << endl << " ...decrypting message... " << endl;

	vector<double> decypherTextNumericForm;

	double value = 0;

	int decriptedMessageLength = 0;

	for (vector<double>::const_iterator i = encriptedMessage.begin(); i != encriptedMessage.end(); i++)
	{
		decriptedMessageLength++;

		cout << endl << endl << " decrypting " << *i << " --> ";

		value = *i;
		value = pow(value, d);

		cout << endl << " exp d = " << value;

		value = fmod(value, n);

		cout << endl << " mod n = " << value << endl;

		decypherTextNumericForm.push_back(value);
	}

	cout << endl << endl << " ...decypher text in integer vector form: " << endl;

	for (vector<double>::const_iterator i = decypherTextNumericForm.begin(); i != decypherTextNumericForm.end(); i++)
	{
		cout << *i << ' ';
	}

	char* decriptedMessage_ptr = new char[decriptedMessageLength];

	cout << endl << endl << " ...decypher text in character array form: " << endl;

	for (int i = 0; i < decriptedMessageLength; i++)
	{
		decriptedMessage_ptr[i] = (char)(decypherTextNumericForm[i] + 64);

		cout << decriptedMessage_ptr[i];
	}

	cout << endl;

	return decriptedMessage_ptr;
}


vector<double> RSA_algorythm(void)
{
	cout << endl << endl << "			--- RSA ALGORITHM ---			" << endl << endl << endl << endl;

	goto recycle;

	recycle:

	vector<double> primeNumbers = RSA_primeNumbers();
	vector<double> plainTextNumericForm;
	vector<double> cypherTextNumericForm;

	array<double, 3> randomPrimes = RSA_selectTwoRandom(primeNumbers);

	double totient = RSA_compute_totient(randomPrimes);
	double e = RSA_compute_e(totient);
	double d = RSA_compute_d(totient, e);

	string plainText;

	cout << endl << " --- Type the message to encrypt, replacyng space char with underscore ( _ ) -> " << endl;

	cin >> plainText; // ask user to insert plain text message 

	cout << endl << " ...plain text typed is: " << endl << plainText << endl;

	int plainTextLength = plainText.length() + 1;

	char* plainTextVector_ptr = new char[plainTextLength];
	 
	memcpy(plainTextVector_ptr, plainText.c_str(), plainTextLength);

	cout << endl << " ...plain text in character array form: " << endl;

	for (int i = 0; i < plainTextLength - 1; i++)
	{
		cout << plainTextVector_ptr[i];
	}

	for (int i = 0; i < plainTextLength - 1; i++)
	{
		if (plainTextVector_ptr[i] - 64 <= 0)
		{
			plainTextNumericForm.push_back(0);
		}
		else
		{
			plainTextNumericForm.push_back(plainTextVector_ptr[i] - 64); // reduce the ASCII character to simplify the next calculation
		}
	}


	cout << endl << endl << " ...plain text in integer vector form: " << endl;

	for (vector<double>::const_iterator i = plainTextNumericForm.begin(); i != plainTextNumericForm.end(); i++)
	{
		cout << *i << ' ';

		if (*i > randomPrimes[2]) // check if message are less than n
		{
			cout << endl << endl << " ATTENTION! ...plain text is longer than n !!" << endl << endl;

			goto recycle; // return in previous position
		}
	}
	
	double value = 0;

	for (vector<double>::const_iterator i = plainTextNumericForm.begin(); i != plainTextNumericForm.end(); i++)
	{
		cout << endl << endl << *i;

		value = pow((*i), e);

		cout << endl << " exp e = " << value;

		value = fmod(value, randomPrimes[2]);

		cout << endl << " mod n = " << value << endl;

		cypherTextNumericForm.push_back(value);
	}

	cout << endl << endl << " ...cypher text in integer vector form: " << endl;

	for (vector<double>::const_iterator i = cypherTextNumericForm.begin(); i != cypherTextNumericForm.end(); i++)
	{
		cout << *i << ' ';
	}

	RSA_decryptMessage(cypherTextNumericForm, d, randomPrimes[2]);

	return cypherTextNumericForm;
}