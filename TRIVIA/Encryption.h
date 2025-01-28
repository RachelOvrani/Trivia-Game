#pragma once


void encrypt_string(char* plan_text);
void decrypt_string(char* encrypted_text);

void encrypt_bytes(char* bytes, int size);
void decrypt_bytes(char* bytes_encrypted, int size);


void encrypt_file(const char* source_file_name_to_encrypt, const char* target_encripted_file_name);
void decrypt_file(const char* source_file_name_to_decrypt, const char* target_decripted_file_name);
