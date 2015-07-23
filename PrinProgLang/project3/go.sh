echo -e "\nsequential"
./spell_seq $1

echo -e "\nt4 single" 
./spell_t4_singleloop $1

echo -e "\nt4 fastest"
./spell_t4_fastest $1

echo -e "\nt8 single"
./spell_t8_singleloop $1

echo -e "\nt8 fastest"
./spell_t8_fastest $1
