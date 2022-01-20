#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 1
//
Transaction::Transaction( std::string ticker_symbol, unsigned int day_date, unsigned int month_date, unsigned year_date, bool buy_sell_trans, 
unsigned int number_shares, double trans_amount){

  symbol = ticker_symbol;
  day = day_date;
  month = month_date;
  year = year_date;
  
  if (buy_sell_trans== true) {
        trans_type = "Buy";
  } else {
        trans_type = "Sell";
    }

  shares = number_shares;
  amount = trans_amount;

  share_balance = 0; 
  acb_per_share = 0.0; 
  cgl = 0.0;
  acb = 0.0; 

trans_id = assigned_trans_id++; 

p_next = nullptr; 
}
 




// Destructor
// TASK 1
//

Transaction::~Transaction() {
}


// Overloaded < operator.
// TASK 2
//

bool Transaction::operator<(Transaction const &other) {

  if(this -> year < other.year){
    return true;
  }
  else if(this -> year == other.year) {
    
    if(this -> month < other.month){
      return true;
    }
  
    else if(this -> month == other.month) {

      if(this -> day < other.day){
        return true;
      }
    
      else{
        return false; 
      }
    

    }
    return false; 
  }
  return false; 
}




// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 3
//

History::History():p_head(nullptr) {
  
}


// Destructor
// TASK 3
//

History::~History(){
  
  while(p_head != nullptr){
      Transaction *p_curr = p_head;
      p_head = p_head->get_next();
      delete p_curr;  
      p_curr = nullptr;
    }

    delete p_head; 
    p_head = nullptr; 

}

// read_history(...): Read the transaction history from file.
// TASK 4
//
// Read the transaction history from file.
//
void History::read_history(){
  ece150::open_file();

    while (ece150::next_trans_entry() == true) {
        Transaction *p_history = new Transaction(ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(), ece150::get_trans_year(), ece150::get_trans_type(), ece150::get_trans_shares(), ece150::get_trans_amount());
        insert(p_history);
    }

  ece150::close_file();
}

// insert(...): Insert transaction into linked list.
// TASK 5
//

// Insert transaction in linked list.
//
void  History::insert(Transaction *p_new_trans){

  if(p_head == nullptr){
    p_new_trans->set_next(nullptr);
    p_head = p_new_trans; 

  }else{

    Transaction *p_temp = p_head; 

    while(p_temp->get_next() != nullptr){
      p_temp = p_temp->get_next(); 
    }
    p_temp->set_next(p_new_trans); 
    p_new_trans->set_next(nullptr);
  }   
}

// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//
// Sort using insertion sort.
//
void  History::sort_by_date(){

  if(p_head == nullptr || p_head->get_next() == nullptr){
    return; 
  }else{
    Transaction *p_sort = nullptr;
    Transaction *p_temp = p_head;

  while(p_head != nullptr){
     
    p_head = p_head -> get_next(); 
    p_temp -> set_next(nullptr); 
    if(p_sort == nullptr){
      p_sort = p_temp;
      p_temp = nullptr;   
    }else{
      Transaction *p_trav = p_sort; 
       while(*(p_trav->get_next()) < *p_temp){
          p_trav = p_trav->get_next(); 
        }
        p_temp->set_next(p_trav->get_next()); 
        p_trav->set_next(p_temp); 
        p_temp = nullptr; 
        p_trav = nullptr;
      
    }

  }
  p_temp = nullptr;
  p_head = p_sort;  
  p_sort = nullptr; 
    
  }
   
}

//    }
  
//   p_head = p_sort; 

// }

// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//
// Update the acb members in the linked list.
//
void  History::update_acb_cgl(){

  double acb = 0.0; 
  double acb_per_share = 0.0; 
  int share_balance = 0; 
  double cgl = 0.0; 

  if(p_head == nullptr){
    return; 
  }else{
    Transaction *p_temp = p_head; 

    while(p_temp != nullptr){
      if(p_temp->get_trans_type() == false){

          acb -= (p_temp->get_shares() * acb_per_share); 
          share_balance -= p_temp->get_shares(); 
          acb_per_share = (acb/share_balance); 
          cgl = (p_temp->get_amount() - (acb_per_share * p_temp->get_shares()));
          p_temp->set_cgl(cgl); 

      }else{
          acb += p_temp->get_amount(); 
          share_balance += p_temp->get_shares(); 
          acb_per_share = (acb/share_balance); 
      }
      p_temp->set_acb_per_share(acb_per_share); 
      p_temp->set_share_balance(share_balance); 
      p_temp->set_acb(acb); 
      p_temp = p_temp->get_next(); 

    }

  }

}



// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8
// Compute the ACB, and CGL.
//
double  History::compute_cgl(unsigned int year){

  double cgl = 0.0; 

  History::update_acb_cgl(); 
  
  if(p_head == nullptr){
    return 0; 
  }else{
    Transaction *p_temp = p_head; 
    while(p_temp != nullptr){
      if (p_temp->get_year() == year){
        cgl += p_temp->get_cgl();

      }
      p_temp = p_temp->get_next();    
    }
  }
  return cgl; 
}

//TASK 9
// Print the transaction history.
//
void  History::print(){
  std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl; 

  Transaction *p_temp = p_head; 

  while(p_temp != nullptr){
    p_temp->print(); 
    p_temp = p_temp->get_next();
   
  }

  std::cout << "========== END TRANSACTION HISTORY ============" << std::endl; 
}



// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
