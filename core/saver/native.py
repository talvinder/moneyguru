# -*- coding: utf-8 -*-
# Created By: Virgil Dupras
# Created On: 2010-01-11
# Copyright 2010 Hardcoded Software (http://www.hardcoded.net)
# 
# This software is licensed under the "HS" License as described in the "LICENSE" file, 
# which should be included with this package. The terms are also available at 
# http://www.hardcoded.net/licenses/hs_license

import xml.etree.cElementTree as ET

def save(filename, accounts, groups, transactions, schedules, budgets):
    def date2str(date):
        return date.strftime('%Y-%m-%d')
    
    def write_transaction_element(parent_element, transaction):
        transaction_element = ET.SubElement(parent_element, 'transaction')
        attrib = transaction_element.attrib
        attrib['date'] = date2str(transaction.date)
        attrib['description'] = transaction.description
        attrib['payee'] = transaction.payee
        attrib['checkno'] = transaction.checkno
        attrib['mtime'] = str(int(transaction.mtime))
        for split in transaction.splits:
            split_element = ET.SubElement(transaction_element, 'split')
            attrib = split_element.attrib
            attrib['account'] = split.account_name
            attrib['amount'] = str(split.amount)
            attrib['memo'] = split.memo
            if split.reference is not None:
                attrib['reference'] = split.reference
            if split.reconciliation_date is not None:
                attrib['reconciliation_date'] = date2str(split.reconciliation_date)
    
    root = ET.Element('moneyguru-file')
    for group in groups:
        group_element = ET.SubElement(root, 'group')
        attrib = group_element.attrib
        attrib['name'] = group.name
        attrib['type'] = group.type
    for account in accounts:
        account_element = ET.SubElement(root, 'account')
        attrib = account_element.attrib
        attrib['name'] = account.name
        attrib['currency'] = account.currency.code
        attrib['type'] = account.type
        if account.group:
            attrib['group'] = account.group.name
        if account.reference is not None:
            attrib['reference'] = account.reference
        if account.account_number:
            attrib['account_number'] = account.account_number
    for transaction in transactions:
        write_transaction_element(root, transaction)
    # the functionality of the line below is untested because it's an optimisation
    scheduled = [s for s in schedules if s.is_alive]
    for recurrence in scheduled:
        recurrence_element = ET.SubElement(root, 'recurrence')
        attrib = recurrence_element.attrib
        attrib['type'] = recurrence.repeat_type
        attrib['every'] = str(recurrence.repeat_every)
        if recurrence.stop_date is not None:
            attrib['stop_date'] = date2str(recurrence.stop_date)
        for date, change in recurrence.date2globalchange.items():
            change_element = ET.SubElement(recurrence_element, 'change')
            change_element.attrib['date'] = date2str(date)
            if change is not None:
                write_transaction_element(change_element, change)
        for date, exception in recurrence.date2exception.items():
            exception_element = ET.SubElement(recurrence_element, 'exception')
            exception_element.attrib['date'] = date2str(date)
            if exception is not None:
                write_transaction_element(exception_element, exception)
        write_transaction_element(recurrence_element, recurrence.ref)
    for budget in budgets:
        budget_element = ET.SubElement(root, 'budget')
        attrib = budget_element.attrib
        attrib['account'] = budget.account.name
        attrib['type'] = budget.repeat_type
        attrib['every'] = unicode(budget.repeat_every)
        attrib['amount'] = unicode(budget.amount)
        if budget.target is not None:
            attrib['target'] = budget.target.name
        attrib['start_date'] = date2str(budget.start_date)
        if budget.stop_date is not None:
            attrib['stop_date'] = date2str(budget.stop_date)
    tree = ET.ElementTree(root)
    tree.write(filename, 'utf-8')